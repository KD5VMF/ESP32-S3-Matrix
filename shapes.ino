#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL 14
#define NUMPIXELS    64  // 8x8 matrix
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define RGB_BRIGHTNESS 20  // Brightness level (0-255)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Define 30 shapes as 8x8 bitmaps
const uint8_t shapes[30][8] = {
  {0b00111100, 0b01111110, 0b11111111, 0b11111111, 0b11111111, 0b01111110, 0b00111100, 0b00000000}, // Circle
  {0b11111111, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b10000001, 0b11111111, 0b00000000}, // Square
  {0b00011000, 0b00111100, 0b01111110, 0b11111111, 0b01111110, 0b00111100, 0b00011000, 0b00000000}, // Diamond
  {0b00111100, 0b01000010, 0b10000001, 0b10000001, 0b10000001, 0b01000010, 0b00111100, 0b00000000}, // Circle Outline
  {0b00011000, 0b00011000, 0b01111110, 0b01111110, 0b01111110, 0b00011000, 0b00011000, 0b00000000}, // Plus
  {0b10000001, 0b01000010, 0b00111100, 0b00011000, 0b00111100, 0b01000010, 0b10000001, 0b00000000}, // X
  {0b11111111, 0b11000011, 0b10111101, 0b10100101, 0b10100101, 0b10111101, 0b11000011, 0b11111111}, // Fancy Square
  {0b01111110, 0b11000011, 0b10100101, 0b10011001, 0b10011001, 0b10100101, 0b11000011, 0b01111110}, // Hexagon
  {0b00011000, 0b00111100, 0b01100110, 0b11000011, 0b01100110, 0b00111100, 0b00011000, 0b00000000}, // Arrow
  {0b00011000, 0b00111100, 0b01111110, 0b11011011, 0b11011011, 0b01111110, 0b00111100, 0b00011000}, // Star
  {0b11111111, 0b00011000, 0b00111100, 0b01111110, 0b00111100, 0b00011000, 0b11111111, 0b00000000}, // Hourglass
  {0b00111100, 0b01100110, 0b11000011, 0b11011011, 0b11011011, 0b01111110, 0b00111100, 0b00000000}, // Smiley Face
  {0b00111100, 0b01000010, 0b10011001, 0b10000001, 0b10011001, 0b01000010, 0b00111100, 0b00000000}, // Heart
  {0b00011000, 0b01111110, 0b11011011, 0b11011011, 0b11011011, 0b01111110, 0b00011000, 0b00000000}, // Cross
  {0b11111111, 0b10000001, 0b10111101, 0b10100101, 0b10100101, 0b10011001, 0b11111111, 0b00000000}, // Bold Square
  {0b01111110, 0b10011001, 0b11011011, 0b11000011, 0b11011011, 0b10011001, 0b01111110, 0b00000000}, // Diamond Filled
  {0b01100110, 0b10011001, 0b11011011, 0b11111111, 0b11111111, 0b11011011, 0b10011001, 0b01100110}, // Checkered
  {0b00111100, 0b01111110, 0b11100111, 0b11011011, 0b11011011, 0b11100111, 0b01111110, 0b00111100}, // Oval
  {0b00011000, 0b00100100, 0b01000010, 0b10000001, 0b01000010, 0b00100100, 0b00011000, 0b00000000}, // Kite
  {0b00011000, 0b00111100, 0b01111110, 0b11111111, 0b01111110, 0b00111100, 0b00011000, 0b00000000}, // Snowflake
  {0b11111111, 0b10000001, 0b10100101, 0b10111101, 0b10111101, 0b10100101, 0b10000001, 0b11111111}, // Rounded Square
  {0b00111100, 0b01111110, 0b11100111, 0b11011011, 0b11100111, 0b01111110, 0b00111100, 0b00000000}, // Squished Diamond
  {0b11100111, 0b11000011, 0b10000001, 0b10011001, 0b10011001, 0b10000001, 0b11000011, 0b11100111}, // Ghost
  {0b00111100, 0b01111110, 0b11100111, 0b11011011, 0b11011011, 0b11100111, 0b01111110, 0b00111100}, // Alien
  {0b00011000, 0b00111100, 0b01111110, 0b11111111, 0b11111111, 0b01111110, 0b00111100, 0b00011000}, // Sun
  {0b11111111, 0b10000001, 0b10000001, 0b10011001, 0b10000001, 0b10000001, 0b11111111, 0b00000000}, // Box with Dot
  {0b00111100, 0b01100110, 0b11000011, 0b11111111, 0b11000011, 0b01100110, 0b00111100, 0b00000000}, // Compass
  {0b11100111, 0b11011011, 0b10011001, 0b10111101, 0b10011001, 0b11011011, 0b11100111, 0b00000000}, // Spider
  {0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b00000000}, // Filled Square
  {0b00011000, 0b00111100, 0b01111110, 0b11111111, 0b01111110, 0b00111100, 0b00011000, 0b00000000}  // Sun with Rays
};

// Define colors for each shape
const uint32_t colors[30] = {
  pixels.Color(255, 0, 0),    // Red
  pixels.Color(0, 255, 0),    // Green
  pixels.Color(0, 0, 255),    // Blue
  pixels.Color(255, 255, 0),  // Yellow
  pixels.Color(0, 255, 255),  // Cyan
  pixels.Color(255, 0, 255),  // Magenta
  pixels.Color(255, 165, 0),  // Orange
  pixels.Color(75, 0, 130),   // Indigo
  pixels.Color(238, 130, 238),// Violet
  pixels.Color(255, 255, 255),// White
  pixels.Color(128, 0, 0),    // Maroon
  pixels.Color(0, 128, 0),    // Dark Green
  pixels.Color(0, 0, 128),    // Navy
  pixels.Color(128, 128, 0),  // Olive
  pixels.Color(0, 128, 128),  // Teal
  pixels.Color(128, 0, 128),  // Purple
  pixels.Color(192, 192, 192),// Silver
  pixels.Color(255, 69, 0),   // Orange Red
  pixels.Color(154, 205, 50), // Yellow Green
  pixels.Color(0, 100, 0),    // Dark Green
  pixels.Color(0, 191, 255),  // Deep Sky Blue
  pixels.Color(25, 25, 112),  // Midnight Blue
  pixels.Color(255, 140, 0),  // Dark Orange
  pixels.Color(107, 142, 35), // Olive Drab
  pixels.Color(75, 0, 130),   // Indigo
  pixels.Color(139, 0, 139),  // Dark Magenta
  pixels.Color(210, 105, 30), // Chocolate
  pixels.Color(240, 230, 140),// Khaki
  pixels.Color(128, 128, 128),// Gray
  pixels.Color(255, 222, 173) // Navajo White
};

void setup() {
  pixels.begin(); // Initialize the NeoPixel library
  pixels.setBrightness(RGB_BRIGHTNESS); // Set brightness for all LEDs
}

void loop() {
  for (int shapeIndex = 0; shapeIndex < 30; shapeIndex++) {
    displayShape(shapes[shapeIndex], colors[shapeIndex]);
    delay(1000); // Display each shape for 1 second
  }
}

void displayShape(const uint8_t shape[8], uint32_t color) {
  pixels.clear();

  // Draw the shape
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      if (shape[y] & (1 << (7 - x))) { // Check if the bit is set
        int pixelIndex = y * MATRIX_WIDTH + x;
        pixels.setPixelColor(pixelIndex, color);
      }
    }
  }

  // Show the updated matrix
  pixels.show();
}
