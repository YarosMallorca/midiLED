# midiLED
**Control WS2812B LED Strips with Teensy from Ableton Live!**

## Installation

**Requirements:**

> Software:
- <a href="https://www.arduino.cc/en/software/OldSoftwareReleases">Arduino IDE (Any version compatible with Teensyduino. Ex. 1.8.5)</a>
- <a href="https://www.pjrc.com/teensy/teensyduino.html">Teensyduino</a>

> Hardware:
- Any teensy with MIDI support (for example Teensy LC)
- WS2812B 5V LED Strip
- Power supply (if long enough LED Strip is used) More details <a href="https://www.temposlighting.com/guides/power-any-ws2812b-setup">Here</a>


## Compiling & Flashing

Before proceeding Install all required software (listed above)

Open Arduino IDE and follow these steps:

**1. <a href="https://github.com/YarostheLaunchpadder/midiLED/archive/refs/heads/main.zip">Download</a> & Open the midiLED.ino**

**2. Set board type to your teensy target (as shown below)**

<img src="https://user-images.githubusercontent.com/54041533/191255504-60e129c8-de62-48aa-badc-d278e134e9a0.png" width=500/>

**3. Set USB Type to MIDI in ```Tools --> USB Type: --> MIDI```**

**4. Set Optimize option in ```Tools --> Optimize``` to ```Fastest```**

**5. In code, set the number of LEDs you're using in line 3, and set the PIN your LED strip is connected to in line 4.<br>Adjust other options if necessary (not recommended)**

**6. Set the port the Teensy is connected in ```Tools --> Port```. Tip: Port COM1 is normally not valid. The correct port will be marked with your microcontroller name**

**7. Build and upload by clicking the button (shown below)**

<img src="https://user-images.githubusercontent.com/54041533/191257247-cb66845f-eda8-490f-95a4-eb590428049d.png" width=400/>

## Common issues

#### LEDs are getting stuck/don't turn off
Play with the rDelay variable in the code, slightly decrease it (by 20 or 30) or increase it. Might fix the issue. Larger, more powerful teensy might potentially solve the issue as well

#### No output
Make sure the PIN in the code is set to the actual PIN that your leds are connected.
Also, the LED strip MUST be WS2812B, other LED strips will NOT work.
Make sure you're actually sending some MIDI to the Teensy
Double-check the polarity of the LED strip, and that you've connected the signal wire to DIN/DI (Digital In) on the LED Strip, not the DO (Digital Output)
