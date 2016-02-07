# Arduhog #

The aim of this project is to create pcb diagrams and software thanks to it will be possible to build hardware cockpit simulator for DCS A10C (and maybe more). PCB board and software try to simulate HID interface (keyboard exactly), so the knowledge that it is available here should also fit for other simulators/games. 

I'm lazy so im using Arduino Nano as main controller, it's cheap (if you are using chinese replacements), it's popular, you don't need any advanced hardware programmer and when you will be bored with this project you can use it for something else.

Project requires [this library](https://git.io/vgGM2) for working. You have to install it in your Arduino IDE.

You can also check [DCS-Bios](http://dcs-bios.a10c.de/), but it have a different approach, it's requires installing some additional scripts in DCS World and it's works only with A10C Cocpit. 

## Project Structure Description ## 

### Hardware ###

  * eagle/ - directory containing EAGLE project; 
  * eagle/main_board - pcb diagrams for main board; mainly it contains all necessary components to simulate hardware keyboard and other stuff to indicate device status, some buttons (currently they do nothing); main board have 8 expansions port for extension board, it allows you to, theoretically, support up to 64 buttons/switches; connection to the PC is done via the USB B port (why B? because it is the simplest USB port for soldering and it fit to 100 mil boards)
  * eagle/ext_board - pcb diagrams for extension board; thanks to this board you can connect up to 16 button; 
  * eagle/ahcp - only schematic, how to connect switches from Armament Hud Control Panel to port what will fit extension board;
  * eagle/bom/* - Bill of Materials for pcb boards;
  * eagle/tests/* - tests and early material created for this project;

### Software ###

  * src/arduhog/* - source code for arduino controller;
  * src/arduhog/keymappings.ods - spreedsheet containing mapping from eagle/ahcp.sch to buttons in code and shortcuts in game.  

## Screenshots, images and movies  ## 
  
  * [Image gallery of prototype](https://imgur.com/a/71ZCE)
  * [Working prototype](https://vid.me/YhLZ)

  





