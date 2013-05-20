ShrimpKey
===========
*Inspired by Shrimping.it and MaKey Makey*

An extended [Shrimp](http://www.shrimping.it/shrimp) which acts like a MaKey Makey. 
See this video on [Vimeo](http://bit.ly/116lLel)

Repository Contents
-------------------
* **/Layout** - Hardware files
* **/ShrimpKey** - Firmware files
* **/UsbKeyboard** - Library for Arduino

Recipe
------
Make the ShrimpKey (see **/Layout/ShrimpKey.pdf**).

Copy the folder **/UsbKeyboard** (and contents) to the libaries-folder of Arduino IDE.

Open **/ShrimpKey/ShrimpKey.ino** in Arduino IDE (make sure that settings.h is also loaded).

Upload to Shrimp (you have to use a [CP2102](http://www.aliexpress.com/store/product/New-Shop-Sale-5pcs-lot-CP2102-Serial-Converter-USB-2-0-To-TTL-UART-6PIN-Module/213957_623537804.html)).

Disconnect the CP2102.

Connect a USB-cable to the ShrimpKey and your computer.

***Have fun!***

License Information
-------------------

The source code and hardware are released under the [Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License](http://creativecommons.org/licenses/by-nc-sa/3.0/) (unless licensed otherwise). 


Sources
-------
* http://shrimping.it/blog/shrimp/
* http://www.practicalarduino.com/projects/virtual-usb-keyboard
* https://github.com/sparkfun/MaKeyMaKey
