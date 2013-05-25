**Warning!**
Never connect a CP2102 and the USB-cable together,
unless you've disconnected the 5V-pin of the CP2102.

Components
----------

Most components can be ordered from your local electronics store or from Tayda Electronics (https://www.taydaelectronics.com/) (probably cheaper).

[A-xxxx = Tayda partno.]

* 1x  A-854	ATMEGA328P-PU
* 2x  A-523	22pF condensator
* 4x  A-4008	100nF condensator
* 1x  A-4534	10uF condensator
* 1x  A-2115	10kOhm resistor
* 1x  A-157	1N4148 diode
* 1x  A-230	16000 MHz crystal
* 1x  A-5144	button
* 2x  A-2251	100Ohm resistor
* 1x  A-5031	Stripboard
* 1x  A-407	LED (blue)
* 1x  A-197	pin header male
* 1x  A-196	pin header female
* 1x  A-199	pin header male (angle)
* 2x  A-4999	wire (blue)
* 18x A-2063	10MOhm resistor
* 1x  A-2087	2.2kOhm resistor
* 2x  A-2086	68Ohm resistor
* 1x  A-437	USB-B female connector
* 1x  A-057	LED (green)

Total cost: approx. $ 5.70 + shipping (Some components need to be ordered in volumes of 10, so the total costs are a bit higher.)

You also need:
* 2x 		3.6V (≤ 0.5W) Zenerdiode (find these in your local electronics store)


The ATMEGA328P-PU's from Tayda don't have a BootLoader on it.
And you also need to upload the firmware to it (you then also need a CP2102: http://www.aliexpress.com/item/New-Shop-Sale-5pcs-lot-CP2102-Serial-Converter-USB-2-0-To-TTL-UART-6PIN-Module/623537804.html).

If you don't know how to burn a BootLoader to it and/or you don't know how to load the firmware, please contact me. If I have ATMEGA328P-PU's in stock, I can send you one with the right BootLoader and Firmware (3,50 EUR + shipping).


Making the ShrimpKey
--------------------

If you've ordered the stripboard from Tayda, you can use the design in **ShrimpKey.pdf** to build the ShrimpKey.

Start with cutting (with a sharp knife) the red line (not the red crosses!) on the copper side of the stripboard (you only have to cut through the copper, not the whole stripboard).

After that, place everything on the non-copper side. You then solder them on the copper side.

The *1N4148 diode*, *3.6V Zenerdiodes* and *10uF condensator* have polarity. 
The diodes have a black line on them, these lines are marked in the layout.
The condensator has a mark for the minus-side, on the layout you can find a - and a +.

All other components can be connected both ways.

After you've soldered everything use a 3.5mm metal drillbit and place it in the holes marked with a red cross. Turn it manually a couple of times until the copper is removed on that spot.

