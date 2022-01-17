<h1 align="center">
  <a href="https://youtu.be/GM8qZVZJs2o"><img src="https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/img/screw_propelled_vehicle.jpg" alt="Screw-propelled vehicle" width="800"></a>
  <br>
    Screw-propelled platform
  <br>
</h1>

<p align="center">
  <a href="https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/README.md">Русский(Russian)</a> •
  <a href="https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/README-en.md">English</a> 
</p>

# Description
Code for testing the screw-propelled platform

# Main parts
* Arduino pro mini;
* 2 x 18650 batteries and battery box for them;
* toggle switch KCD-11;
* L298N motor driver;
* yellow arduino TT gearmotors;
* HW-517 or other transistor switch;
* HC-06 bluetooth module (optional);
* KY-022 IR receiver and remote control for it;
* piece of plywood 305x170x6 mm;
* [screws, holders, cabin, etc.] (https://www.thingiverse.com/thing:5204085)

### Fasteners
* Bearing 623 2RS (180023) 3dx10Dx4H (mm) x2;
* Self-tapping screw DIN7982 3.5x9.5	x2;
* Self-tapping screw DIN7982 2.9x16	x12;
* Self-tapping screw DIN7981 2.2x9.5	x12;
* Self-tapping screw DIN7981 2.9x9.5	x12;
* Screw DIN7985 M3x25		x6;
* Nut DIN934 M3x0.5		x8;

# Wiring diagram
![Wiring diagram](https://github.com/TrashRobotics/ScrewPropelledVehicle/blob/main/img/schematic.png)

# Sketch 
Sketch for Arduino pro mini: **screw_propelled_vehicle/screw_propelled_vehicle.ino**
If you plan to control using an IR remote control, then in the sketch itself you need to change the buttons codes for your IR remote control.

**bluetoothtester.py** - an optional python script to control the platform from a PC.
To run the script, you need to install the punput package.
The platform is controlled from the PC by arrows.

For more details watch the video.

