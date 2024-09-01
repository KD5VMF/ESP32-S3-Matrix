#include <Adafruit_NeoPixel.h>
#include "font.h"  // Include the font header file

#define PIN_NEOPIXEL 14
#define NUMPIXELS    64  // 8x8 matrix
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define RGB_BRIGHTNESS 50  // Brightness level (0-255)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin(); // Initialize the NeoPixel library
  pixels.setBrightness(RGB_BRIGHTNESS); // Set brightness for all LEDs
}

void loop() {
  scrollText("ADAM FIGUEROA...  ", pixels.Color(0, 255, 0));  // Scroll text in Green
}

void scrollText(const char* text, uint32_t color) {
  int len = strlen(text);

  for (int shift = 0; shift < len * 8 + MATRIX_WIDTH; shift++) {
    pixels.clear();  // Clear the matrix before drawing the next frame

    for (int i = 0; i < len; i++) {
      char c = text[i];
      const uint8_t* bitmap;

      if (c >= '0' && c <= '9') {
        bitmap = numbers[c - '0'];
      } else if (c >= 'A' && c <= 'Z') {
        bitmap = letters[c - 'A'];
      } else if (c == ' ') {
        bitmap = specialChars[0];  // Space
      } else if (c == '!') {
        bitmap = specialChars[1];  // Exclamation mark
      } else {
        continue;  // Skip unsupported characters
      }

      for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
          int columnPosition = i * 8 + x - shift;

          if (columnPosition >= 0 && columnPosition < MATRIX_WIDTH) {
            if (bitmap[y] & (1 << (7 - x))) {
              int pixelIndex = y * MATRIX_WIDTH + columnPosition;
              pixels.setPixelColor(pixelIndex, color);
            }
          }
        }
      }
    }

    pixels.show();
    delay(100);  // Adjust speed of scrolling
  }
}
