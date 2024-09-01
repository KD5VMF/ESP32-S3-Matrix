#include <Adafruit_NeoPixel.h>

// Define your matrix size and pin configuration
#define MATRIX_PIN 14
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUMPIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

// Define the correct heart shape
const uint8_t heartShape[] = {
  0b00000000,  // ........
  0b01100110,  //  **  **
  0b11111111,  // ********
  0b11111111,  // ********
  0b11111111,  // ********
  0b01111110,  //  ******
  0b00111100,  //   ****
  0b00011000   //    **
};

void setup() {
  matrix.begin();
  matrix.setBrightness(20); // Adjust brightness if needed
  matrix.clear();
  
  drawHeart();
  
  matrix.show(); // Display the heart
}

void loop() {
  // No continuous updates required as the heart is static
}

// Function to draw the heart using your existing drawing style
void drawHeart() {
  uint32_t color = matrix.Color(0, 255, 0); // Red color GRB For some reason??!
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      if (heartShape[y] & (1 << (7 - x))) {
        drawPixel(x, y, color);
      }
    }
  }
}

// Function to draw a single pixel on the matrix
void drawPixel(int x, int y, uint32_t color) {
  int pixelIndex = y * MATRIX_WIDTH + x;
  matrix.setPixelColor(pixelIndex, color);
}
