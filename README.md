# Paddle-of-Theseus
This Repository holds the design work for Hidden Layer Design's "Paddle of Theseus" project.

Please take a look at [the Wiki](https://github.com/ChaseStewart/Paddle-of-Theseus/wiki) for in-depth information.

## Author
[Chase E Stewart](https://chasestewart.co) for [Hidden Layer Design](https://hiddenlayerdesign.com)

## Repository Contents
* **/Software/PadleFirmware** - Software Arduino project to program the Teensy LC/4.0. This Teensy is the main microcontroller for this project, and uses input sensor data to output MIDI commands to an external controller.
* **/Software/ClientApp** - Software [FBS](https://build-system.fman.io/)-based [PyQtSA](https://github.com/spencerarrasmith/PyQtSA) project that builds a GUI application to allow users to configure the Paddle's saved settings in EEPROM.
* **/Software/StubPaddle** - Software Python program for ClientApp development to simulate a USB connection to the paddle. Requires a virtual COM port program on Windows to run.
* **/Hardware/Fritzing**   - Fritzing file for project Hardware _NOTE: HW Diagram uses a Teensy3.1 but I have verified proper function on Teensy4.0 and TeensyLC_
* **/Hardware/Diagrams**   - HW diagrams generated from the Fritzing file; currently these include a schematic, PCB image, and breadboard diagram
* **/Hardware/Components** - This contains the bill-of-materials for the relevant *electronics only* for this product. 

## Features
This project will create a bass-guitar-like skeumorphic MIDI instrument out of a standard rowing paddle (or arbitrary instrument body) and sensors. The Teensy microcontroller will take in sensor values and provide MIDI output to an external Audio DAW (In our case, we use Ableton Live and sometimes https://midi.city).

## Implementation Requirements
Implementing this project will require some knowledge of electronics, prototyping, and physical fabrication.

### Software Requirements
- [Arduino IDE](https://www.arduino.cc/en/Main/Software)
- [Arduino Teensyduino add-on](https://www.pjrc.com/teensy/td_download.html)
- [Arduino Teensy Loader](https://www.pjrc.com/teensy/loader.html)

### Hardware
The following is required to build this project.
- EITHER 1x [Teensy 4.0](https://www.pjrc.com/teensy-4-0/) OR
- 1x [Teensy LC](https://www.pjrc.com/teensy/teensyLC.html)
- 1x USB Micro cable
- 1x [Ultrasonic Rangefinder](https://www.rpelectronics.com/sen136b5b-ultrasonic-distance-sensor-module.html) or equivalent
- 1x [SparkFun Triple Axis Accelerometer Breakout](https://www.sparkfun.com/products/12756) or equivalent
- 1x [ThinPot Linear Potentiometer](https://www.digikey.com/en/product-highlight/s/spectra-symbol/thinpot-potentiometers)
- 4x [Instantaneous Capacitive Touch Sensors](https://www.adafruit.com/product/1374) **NOTE: This may become 5 of the same model**
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
  - Set `Tools>Board="Teensy 4.0"` OR `Tools>Board="Teensy LC"` to match the board you are using
  - Set `Tools>USB Type="Serial + MIDI"`
- Compile this project with `"Verify"`, then Flash with `Upload`
  - If needed, press the physical button on the Teensy to enable the bootloader. Ensure Serial connections are closed before hitting `Upload`

## Links
- Personal Website: https://chasestewart.co/
- Hidden Layer Design Website: https://HiddenLayerDesign.com
- Repository: https://github.com/ChaseStewart/Paddle-of-Theseus/

## License
<a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-sa/4.0/80x15.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/">Creative Commons Attribution-ShareAlike 4.0 International License</a>.
