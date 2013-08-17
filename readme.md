All information, including detailed building instructions, can be found on [fromScratchEd.nl](http://fromScratchEd.nl).


ShrimpKey
===========
*Inspired by Shrimping.it and MaKey Makey*

An extended [Shrimp](http://www.shrimping.it/blog/shrimp) which acts like a MaKey Makey. 

See this video on [Vimeo](http://bit.ly/116lLel).


Contact
-------
If you make a ShrimpKey, please let me know!

Twitter: @fromScratchEd

E-mail: info {at} ScratchEd {dot} eu

Link
----
Please use this link when you want to link to my ShrimpKey-pages (GitHub, Vimeo, etc.):

http://bit.ly/17V8SfT

Repository Contents
-------------------
* **/Layout** - Hardware files
* **/ShrimpKey** - Firmware files
* **/UsbKeyboard** - Library for Arduino IDE

Recipe
------
Make the ShrimpKey (see **/Layout**).

Copy the folder **/UsbKeyboard** (and contents) to the libaries-folder of Arduino IDE (v1.0.4 or higher).

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
