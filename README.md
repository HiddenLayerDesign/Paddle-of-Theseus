# Paddle-of-Theseus
This Repository holds the design work for Hidden Layer Design's "Paddle of Theseus" project.

### Hardware Diagram
![Fritzing Diagram of "Paddle of Theseus" Hardware](https://github.com/ChaseStewart/Paddle-of-Theseus/blob/master/Hardware/Diagrams/Paddle_of_Theseus_bb.png)

## Author
[Chase E Stewart](mailto:chase@hiddenlayerdesign.com) for [Hidden Layer Design](https://hiddenlayerdesign.com)

## Repository Contents
* **/Software/Main** - Software Arduino Project to program the Teensy 4.0. This Teensy is the main microcontroller for this project, and uses input sensor data to output MIDI commands to an external controller.
* **/Documentation** - _TODO this will have connection diagrams, etc._
* **/Hardware/Fritzing**  - Fritzing file for project Hardware _NOTE: HW Diagram uses a Teensy3.1 but I own a Teensy4.0_
* **/Hardware/Diagrams**  - HW diagrams generated from the Fritzing file; currently I have a schematic and breadboard diagram

## Features
This project will create a bass-guitar-like skeumorphic MIDI instrument out of a standard rowing paddle (or arbitrary instrument body) and sensors. The Teensy 4.0 microcontroller will take in sensor values and provide MIDI output to an external Audio DAW (In our case, we use Ableton Live).

## Requirements
Implementing this project will require some knowledge of electronics, prototyping, and potentially phyiscal fabrication. 

_Note: Several of the sensors in this project including the 6DoF_IMU and the SEN136B5B are old and discontinued. It would likely be worth extending this project to utilize sensors that are more future-proof. Modernizing the sensors isn't a high priority for me right now, but also would not take much effort._

### Software Requirements
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Arduino Teensyduino addon](https://www.pjrc.com/teensy/td_download.html)
- [Arduino Teensy Loader](https://www.pjrc.com/teensy/loader.html)

### Hardware
You will need the following to run this project
- 1x [Teensy 4.0](https://www.pjrc.com/teensy-4-0/)
- 1x USB Micro cable
- 1x [Ultrasonic Rangefinder](https://www.rpelectronics.com/sen136b5b-ultrasonic-distance-sensor-module.html) or equivalent
- 1x [6DoF IMU](https://www.sparkfun.com/products/retired/10121) or equivalent
- 1x [ThinPot Linear Potentiometer](https://www.digikey.com/en/product-highlight/s/spectra-symbol/thinpot-potentiometers)
- 1x [Instantaneous Capacitive Touch Sensors](https://www.adafruit.com/product/1374) **NOTE: This may become 4 of the same model**
- 1x [RGB Common-Anode Illuminated Rotary Encoder](https://www.sparkfun.com/products/15141)
- 1x [Clear Plastic Rotary Encoder Knob](https://www.sparkfun.com/products/10597)
- 1x [Illuminated Rotary Encoder Breakout Board](https://www.sparkfun.com/products/11722)
- 1x [USB Panel Mount Extension Cable](https://www.adafruit.com/product/3258)
- Audio Software that supports MIDI (I use Ableton Live 8)

### Assembly
Assemble the Sensors and Teensy according to the schematic and breadboard diagrams in */Hardware/Diagrams*, paying special attention to the labeled inputs.

## Getting Started
- Download All Software Requirements above
  - Install Arduino IDE
  - Install Teensy Loader
  - Install Teensyduino SW libraries
- Clone this Repository
- Open `/Software/Main/main.ino` in Arduino 
- Configure Arduino Settings
  - Set `Tools>Board="Teensy 4.0"`
  - Set `Tools>USB Type="Serial + MIDI"`
- Compile this project with `"Verify"`, then Flash with `Upload`
  - If needed, press the physical button on the Teensy 4.0 to enable the bootloader

## Links
- Personal Website: https://chasestewart.co/
- Hidden Layer Design Website: https://HiddenLayerDesign.com
- Repository: https://github.com/ChaseStewart/Paddle-of-Theseus/

## License
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.