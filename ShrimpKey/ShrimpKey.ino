/*
 ************************************************
 **************** ShrimpKey *********************
 ************************************************
 
 ////////////////////////////////////////////////
 /////////////HOW TO EDIT THE KEYS //////////////
 ////////////////////////////////////////////////
 - Edit keys in the settings.h file
 - that file should be open in a tab above (in Arduino IDE)
 
 ////////////////////////////////////////////////
 /////////// ShrimpKey FIRMWARE v1.0.0 //////////
 ////////////////////////////////////////////////
 by Sjoerd Dirk Meijer, info@scratched.eu
 Derived from MakeyMakey Firmware v.1.4.1 and
 www.practicalarduino.com/projects/virtual-usb-keyboard
 This work is licensed under the Creative Commons
 Attribution-NonCommercial-ShareAlike 3.0 Unported License
 (unless licensed otherwise).
 To view a copy of this license, visit
 www.creativecommons.org/licenses/by-nc-sa/3.0/
 */

/////////////////////////
// DEBUG DEFINITIONS ////               
/////////////////////////
//#define DEBUG
//#define DEBUG2 
//#define DEBUG3 
//#define DEBUG_TIMING
//#define DEBUG_TIMING2

#include <UsbKeyboard.h>
#include <oddebug.h>
#include <usbconfig.h>
#include <usbdrv.h>
#include <usbportability.h>

////////////////////////
// DEFINED CONSTANTS////
////////////////////////

#define BUFFER_LENGTH    3     // 3 bytes gives us 24 samples
#define NUM_INPUTS       9    // 6 on the front + 12 on the back
//#define TARGET_LOOP_TIME 694   // (1/60 seconds) / 24 samples = 694 microseconds per sample 
//#define TARGET_LOOP_TIME 758  // (1/55 seconds) / 24 samples = 758 microseconds per sample 
#define TARGET_LOOP_TIME 744  // (1/56 seconds) / 24 samples = 744 microseconds per sample 

#include "settings.h"

/////////////////////////
// STRUCT ///////////////
/////////////////////////
typedef struct {
  byte pinNumber;
  int keyCode;
  byte measurementBuffer[BUFFER_LENGTH]; 
  boolean oldestMeasurement;
  byte bufferSum;
  boolean pressed;
  boolean prevPressed;
  boolean isKey;
} 

ShrimpKeyInput;
ShrimpKeyInput inputs[NUM_INPUTS];

///////////////////////////////////
// VARIABLES //////////////////////
///////////////////////////////////
int bufferIndex = 0;
byte byteCounter = 0;
byte bitCounter = 0;

int pressThreshold;
int releaseThreshold;
boolean inputChanged;
int lastKeyPressed = -1;
int keysPressed = 0;

// Pin Numbers
int pinNumbers[NUM_INPUTS] = {
4, 5, 6, 8,
9, 11, A0, A2, A5
};

// input status LED pin numbers
const int inputLED_a = 12;
const int inputLED_b = 10;
const int inputLED_c = 11;
const int outputK = 13;
const int outputM = 16;
byte ledCycleCounter = 0;

const int ledPin = 12;

// timing
int loopTime = 0;
int prevTime = 0;
int loopCounter = 0;

boolean keyPressed = 0;
  
  
///////////////////////////
// FUNCTIONS //////////////
///////////////////////////
void initializeArduino();
void initializeInputs();
void updateMeasurementBuffers();
void updateBufferSums();
void updateBufferIndex();
void updateInputStates();
void addDelay();
void danceLeds();
void updateOutLEDs();


//////////////////////
// SETUP /////////////
//////////////////////
void setup() 
{
  //V-USB
  // Disable timer0 since it can mess with the USB timing. Note that
  // this means some functions such as delay() will no longer work.
  TIMSK0&=!(1<<TOIE0);

  // Clear interrupts while performing time-critical operations
  cli();
  // Force re-enumeration so the host will detect us
  usbDeviceDisconnect();
  delayMs(250);
  usbDeviceConnect();

  // Set interrupts again
  sei();  
  
  initializeArduino();
  initializeInputs();
  danceLeds();
}

////////////////////
// MAIN LOOP ///////
////////////////////
void loop() 
{
  updateMeasurementBuffers();
  updateBufferSums();
  updateBufferIndex();
  updateInputStates();
  updateOutLEDs();
  addDelay();
}

//V-USB
/**
 * Define our own delay function so that we don't have to rely on
 * operation of timer0, the interrupt used by the internal delay()
 */
void delayMs(unsigned int ms)
{
  for (int i = 0; i < ms; i++) {
    delayMicroseconds(1000);
  }
}

//////////////////////////
// INITIALIZE ARDUINO
//////////////////////////
void initializeArduino() {
#ifdef DEBUG
  Serial.begin(9600);  // Serial for debugging
  Serial.println(111);
#endif

  /* Set up input pins 
   DEactivate the internal pull-ups, since we're using external resistors */
  for (int i=0; i<NUM_INPUTS; i++)
  {
    pinMode(pinNumbers[i], INPUT);
    digitalWrite(pinNumbers[i], LOW);
  }

  pinMode(outputK, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(outputK, LOW);
  digitalWrite(ledPin, LOW);

#ifdef DEBUG2
  delay(4000); // allow us time to reprogram in case things are freaking out
#endif
}

///////////////////////////
// INITIALIZE INPUTS
///////////////////////////
void initializeInputs() {

  float thresholdPerc = SWITCH_THRESHOLD_OFFSET_PERC;
  float thresholdCenterBias = SWITCH_THRESHOLD_CENTER_BIAS/50.0;
  float pressThresholdAmount = (BUFFER_LENGTH * 8) * (thresholdPerc / 100.0);
  float thresholdCenter = ( (BUFFER_LENGTH * 8) / 2.0 ) * (thresholdCenterBias);
  pressThreshold = int(thresholdCenter + pressThresholdAmount);
  releaseThreshold = int(thresholdCenter - pressThresholdAmount);

#ifdef DEBUG
  Serial.println(pressThreshold);
  Serial.println(releaseThreshold);
#endif

  for (int i=0; i<NUM_INPUTS; i++) {
    inputs[i].pinNumber = pinNumbers[i];
    inputs[i].keyCode = keyCodes[i];

    for (int j=0; j<BUFFER_LENGTH; j++) {
      inputs[i].measurementBuffer[j] = 0;
    }
    inputs[i].oldestMeasurement = 0;
    inputs[i].bufferSum = 0;

    inputs[i].pressed = false;
    inputs[i].prevPressed = false;

    inputs[i].isKey = false;

    if (inputs[i].keyCode < 0) {
    } 
    else {
      inputs[i].isKey = true;
    }
#ifdef DEBUG
    Serial.println(i);
#endif
  }
}

//////////////////////////////
// UPDATE MEASUREMENT BUFFERS
//////////////////////////////
void updateMeasurementBuffers() {
  for (int i=0; i<NUM_INPUTS; i++) {
    // store the oldest measurement, which is the one at the current index,
    // before we update it to the new one 
    // we use oldest measurement in updateBufferSums
    byte currentByte = inputs[i].measurementBuffer[byteCounter];
    inputs[i].oldestMeasurement = (currentByte >> bitCounter) & 0x01; 

    // make the new measurement
    boolean newMeasurement = digitalRead(inputs[i].pinNumber);

    // invert so that true means the switch is closed
    newMeasurement = !newMeasurement; 

    // store it    
    if (newMeasurement) {
      currentByte |= (1<<bitCounter);
    } 
    else {
      currentByte &= ~(1<<bitCounter);
    }
    inputs[i].measurementBuffer[byteCounter] = currentByte;
  }
}

///////////////////////////
// UPDATE BUFFER SUMS
///////////////////////////
void updateBufferSums() {
  // the bufferSum is a running tally of the entire measurementBuffer
  // add the new measurement and subtract the old one

  for (int i=0; i<NUM_INPUTS; i++) {
    byte currentByte = inputs[i].measurementBuffer[byteCounter];
    boolean currentMeasurement = (currentByte >> bitCounter) & 0x01; 
    if (currentMeasurement) {
      inputs[i].bufferSum++;
    }
    if (inputs[i].oldestMeasurement) {
      inputs[i].bufferSum--;
    }
  }  
}

///////////////////////////
// UPDATE BUFFER INDEX
///////////////////////////
void updateBufferIndex() {
  bitCounter++;
  if (bitCounter == 8) {
    bitCounter = 0;
    byteCounter++;
    if (byteCounter == BUFFER_LENGTH) {
      byteCounter = 0;
    }
  }
}

///////////////////////////
// UPDATE INPUT STATES
///////////////////////////
void updateInputStates() {
  UsbKeyboard.update();
  inputChanged = false;
  for (int i=0; i<NUM_INPUTS; i++) {
    if (inputs[i].pressed) {
      if (inputs[i].bufferSum < releaseThreshold) {
        inputChanged = true;
        inputs[i].pressed = false;
        UsbKeyboard.releaseKeyStroke();
        keysPressed = 0;        
      }
    if (lastKeyPressed != i) {
      inputs[lastKeyPressed].pressed = false;
      keysPressed = 0;
      inputChanged = false;
      UsbKeyboard.releaseKeyStroke();
    } 
    }
    else if (!inputs[i].pressed) {
      if (inputs[i].bufferSum > pressThreshold) {  // input becomes pressed
        inputChanged = true;
        inputs[i].pressed = true;
        if (lastKeyPressed != i) {
          keysPressed += 1;
          inputs[lastKeyPressed].pressed = false;
          UsbKeyboard.sendKeyStroke(keyCodes[i]);
          lastKeyPressed = i;
        }
      }
    }
    if (keysPressed == 0) {
      if (inputs[i].pressed) {
        inputs[i].pressed = false;
        lastKeyPressed = -1;
      }
    }
 }
#ifdef DEBUG3
  if (inputChanged) {
    Serial.println("change");
  }
#endif
}

///////////////////////////
// ADD DELAY
///////////////////////////
void addDelay() {

  loopTime = micros() - prevTime;
  if (loopTime < TARGET_LOOP_TIME) {
    int wait = TARGET_LOOP_TIME - loopTime;
    delayMicroseconds(wait);
  }

  prevTime = micros();

#ifdef DEBUG_TIMING
  if (loopCounter == 0) {
    int t = micros()-prevTime;
    Serial.println(t);
  }
  loopCounter++;
  loopCounter %= 999;
#endif

}

///////////////////////////
// DANCE LEDS
///////////////////////////
void danceLeds() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delayMs(250);
  pinMode(outputK, OUTPUT);
  digitalWrite(outputK, HIGH);
  digitalWrite(ledPin, LOW);
  delayMs(150);
  digitalWrite(outputK, LOW);
}

///////////////////////////
// update OutLEDs
///////////////////////////
void updateOutLEDs()
{
  if (keyPressed)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}






