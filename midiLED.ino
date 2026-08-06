#include <Adafruit_NeoPixel.h>

#define _numLED 128  // Number of LEDs in your underlights device
#define pinLED  4    // DIN Pin
#define pStart  36   // First note in array (36 is default, maps to C1 in Ableton)
#define bLED    100   // Maximum Overall Brightness of LEDs (0-255) [Set to 255 if you have a power supply]
#define rDelay  80   // Refresh Delay (lower = more FPS) if set too low will lag on high LED Counts
#define ledsPerChannel 127 // Number of Leds controlled per one MIDI Channel

// SysEx protocol
//
// Unlike the note-based path below (which indexes a fixed 128-colour palette
// through velocity), SysEx lets the host set fully arbitrary RGB per LED.
//
//   Set pixels:  F0 7D 00 [idx r g b] [idx r g b] ... F7
//                idx = LED index (0-127), r/g/b = colour channel (0-63)
//   Clear all:   F0 7D 01 F7
//
// 0x7D is the SysEx manufacturer ID reserved for non-commercial / prototype use
#define SYSEX_MANUFACTURER 0x7D
#define SYSEX_CMD_SET      0x00
#define SYSEX_CMD_CLEAR    0x01
#define SYSEX_BUFFER_SIZE  1024

const byte _r[128] = {0, 65, 130, 255, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 134, 81, 40, 20, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 73, 0, 0, 0, 45, 0, 0, 0, 105, 45, 24, 12, 255, 255, 130, 65, 255, 255, 130, 65, 255, 150, 117, 32, 0, 0, 0, 0, 0, 16, 125, 28, 255, 186, 174, 97, 12, 0, 0, 0, 24, 89, 174, 40, 255, 134, 113, 0, 57, 85, 53, 89, 49, 105, 210, 255, 255, 182, 142, 130, 57, 0, 12, 20, 20, 101, 130, 219, 215, 255, 158, 101, 20, 219, 125, 154, 142, 61, 113, 223, 158, 53, 24, 4, 182, 61, 178, 73};
const byte _g[128] = {0, 65, 130, 255, 61, 0, 0, 0, 186, 61, 32, 16, 174, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 194, 166, 85, 45, 134, 85, 45, 24, 36, 0, 0, 0, 53, 0, 0, 0, 61, 0, 0, 0, 65, 0, 0, 0, 12, 53, 81, 53, 57, 73, 20, 0, 69, 0, 125, 28, 0, 255, 235, 255, 138, 255, 166, 40, 0, 0, 24, 16, 49, 223, 255, 255, 255, 255, 255, 138, 81, 81, 28, 0, 69, 166, 255, 89, 40, 73, 77, 20, 28, 57, 0, 65, 73, 190, 223, 178, 20, 210, 235, 150, 101, 61, 113, 255, 0, 0, 206, 65, 174, 49, 81, 20};
const byte _b[128] = {0, 65, 130, 255, 61, 0, 0, 0, 105, 0, 0, 0, 45, 0, 0, 0, 49, 0, 0, 0, 73, 0, 0, 0, 93, 24, 12, 4, 89, 85, 45, 24, 182, 150, 73, 36, 255, 255, 130, 65, 255, 255, 130, 65, 255, 255, 130, 65, 251, 255, 130, 65, 255, 255, 130, 65, 109, 81, 40, 20, 0, 0, 0, 4, 0, 24, 109, 255, 77, 202, 125, 28, 0, 45, 4, 8, 0, 93, 255, 255, 255, 255, 121, 0, 0, 4, 20, 0, 36, 109, 202, 255, 194, 231, 255, 89, 0, 0, 0, 4, 0, 12, 32, 40, 89, 24, 0, 40, 16, 36, 45, 12, 45, 105, 138, 255, 255, 61, 113, 255, 0, 0, 0, 0, 0, 0, 0, 0};

Adafruit_NeoPixel _LED = Adafruit_NeoPixel(_numLED, pinLED, NEO_GRB + NEO_KHZ800);

int count = 0;

byte sysexBuffer[SYSEX_BUFFER_SIZE];
unsigned int sysexLength = 0;

// Expand a 6-bit colour channel (0-63) to the NeoPixels' 8-bit range (0-255).
inline byte scaleColor(byte value) {
  if (value > 63) value = 63;
  return (byte)((value * 255 + 31) / 63);
}

void noteOn(byte ch, byte p, byte v) {
  _LED.setPixelColor((p + (ledsPerChannel * (ch-1))) - pStart, _r[v], _g[v], _b[v]);
  count = 0;
}

void noteOff(byte ch, byte p, byte v) {
  _LED.setPixelColor((p + (ledsPerChannel * (ch-1))) - pStart, 0, 0, 0);
  count = 0;
}

void processSysEx(const byte *data, unsigned int length) {
  // Smallest valid message is F0 <mfr> <cmd> F7
  if (length < 4 || data[0] != 0xF0 || data[1] != SYSEX_MANUFACTURER) return;

  byte command = data[2];

  if (command == SYSEX_CMD_SET) {
    // Body is a run of [idx r g b] quads between the header and the closing F7
    // (which sits at data[length - 1]).
    for (unsigned int i = 3; i + 4 <= length - 1; i += 4) {
      byte idx = data[i];
      if (idx < _numLED)
        _LED.setPixelColor(idx, scaleColor(data[i + 1]), scaleColor(data[i + 2]), scaleColor(data[i + 3]));
    }
    _LED.show();

  } else if (command == SYSEX_CMD_CLEAR) {
    _LED.clear();
    _LED.show();
  }
}

void handleSysEx(const byte *data, uint16_t length, bool complete) {
  for (uint16_t i = 0; i < length; i++) {
    if (sysexLength < SYSEX_BUFFER_SIZE)
      sysexBuffer[sysexLength++] = data[i];
  }

  if (complete) {
    processSysEx(sysexBuffer, sysexLength);
    sysexLength = 0;
  }
}
void setup() {
  _LED.begin();
  _LED.setBrightness(bLED);
  usbMIDI.setHandleNoteOn(noteOn);
  usbMIDI.setHandleNoteOff(noteOff);
  usbMIDI.setHandleSystemExclusive(handleSysEx);
  _LED.show();
}

void loop() {
  while (usbMIDI.read());
  count++;
  if (count == rDelay) {
    _LED.show();
  }
}
