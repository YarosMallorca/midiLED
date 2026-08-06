# midiLED

Control WS2812B LED Strips with Teensy from your favourite MIDI Software

## Showcase

<a href="https://youtu.be/8itP0UnSWBk"><img src="https://img.youtube.com/vi/8itP0UnSWBk/0.jpg" height="200px" /></a>
<a href="https://youtu.be/HWQFZ31jTJc"><img src="https://img.youtube.com/vi/HWQFZ31jTJc/0.jpg" height="200px" /></a>
<a href="https://youtu.be/BP8y9pDiv3k"><img src="https://img.youtube.com/vi/BP8y9pDiv3k/0.jpg" height="200px" /></a>

## Installation

**Requirements:**

Software:

- [Arduino IDE (Version 2 or newer)](https://www.arduino.cc/en/software)
- [Teensyduino](https://www.pjrc.com/teensy/td_download.html)

Hardware:

- Teensy with MIDI support (Teensy 2.0, Teensy++ 2.0, Teensy LC and Teensy 3.0 to 4.1)
- WS2812B 5V LED Strip
- Power supply (if long enough LED Strip is used) [More details here](https://www.temposlighting.com/guides/power-any-ws2812b-setup)

## Downloading and Compiling

> Note: There are different versions of midiLED for various purposes, [check them out](https://github.com/YarosMallorca/midiLED/branches) if you feel like you're missing a feature.
> If you want a feature implemented, please [submit this form](https://github.com/YarosMallorca/midiLED/issues/new?assignees=YarosMallorca&labels=enhancement&template=feature_request.md)
>
> If you've spotted a bug, please [report here](https://github.com/YarosMallorca/midiLED/issues/new?assignees=YarosMallorca&labels=bug&template=bug_report.md)

1. Download and install the Arduino IDE from the link above
2. Download and open [midiLED.ino](https://github.com/YarosMallorca/midiLED/archive/refs/heads/main.zip) file in the Arduino IDE
3. Go to File -> Preferences and add `https://www.pjrc.com/teensy/td_libs_ArduinoIDE/package_Teensyduino_index.json` to the Additional Boards Manager URLs
<img src="https://github.com/user-attachments/assets/a77cf372-a199-4c82-bb5c-b5718d35316b" height="200px"/>
<img src="https://github.com/user-attachments/assets/7e1d89a2-0a33-4e68-9ed6-2a510a0f8601" height="200px"/>

4. Go to the Board Manager (left sidebar), search for `Teensy` and install the latest version
<img src="https://github.com/user-attachments/assets/6b391030-fb8f-4e99-99bd-a15626225066" height="200px"/>

5. Select your Teensy board in the Tools -> Board, select the COM port that it is connected to in Tools -> Port
6. Select the USB Type to MIDI in Tools -> USB Type
7. Set the Optimize option in Tools -> Optimize to Fastest
8. Set the number of LEDs you're using in line 3, and set the PIN your LED strip is connected to in line 4. Adjust other options if necessary.

## Full RGB over SysEx

In addition to the note-based palette (velocity picks one of 128 preset colours),
this firmware accepts **arbitrary per-LED RGB** over MIDI SysEx.

SysEx protocol (manufacturer ID `0x7D`, reserved for prototype/non-commercial use):

| Message    | Bytes                                   | Meaning                                         |
| ---------- | --------------------------------------- | ----------------------------------------------- |
| Set pixels | `F0 7D 00 [idx r g b] [idx r g b] … F7` | `idx` = LED 0-127, `r/g/b` = 0-63 colour channel |
| Clear all  | `F0 7D 01 F7`                           | Turn every LED off                              |

Colour channels are sent as 6-bit values, the firmware expands them to the NeoPixels' full 8-bit range.

## Common issues

### LEDs are getting stuck/don't turn off

Play with the rDelay variable in the code, slightly decrease it (by 20 or 30) or increase it. Might fix the issue. Larger, more powerful teensy might potentially solve the issue as well

### No output

Make sure the PIN in the code is set to the actual PIN that your leds are connected.
Also, the LED strip MUST be WS2812B, other LED strips will NOT work.
Make sure you're actually sending some MIDI to the Teensy
Double-check the polarity of the LED strip, and that you've connected the signal wire to DIN/DI (Digital In) on the LED Strip, not the DO (Digital Output)
