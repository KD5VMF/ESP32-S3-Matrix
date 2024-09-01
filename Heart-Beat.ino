#include <Adafruit_NeoPixel.h>

// Define your matrix size and pin configuration
#define MATRIX_PIN 14
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUMPIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

// Define a broader range of red shades in GRB format for a smooth beating effect
const uint32_t redShades[10] = {
  matrix.Color(0, 255, 0),   // Bright Red
  matrix.Color(0, 220, 0),   // Medium Bright Red
  matrix.Color(0, 190, 0),   // Slightly Dimmer Red
  matrix.Color(0, 160, 0),   // Medium Red
  matrix.Color(0, 130, 0),   // Slightly Dimmer Red
  matrix.Color(0, 100, 0),   // Medium Dim Red
  matrix.Color(0, 70, 0),    // Dim Red
  matrix.Color(0, 50, 0),    // Very Dim Red
  matrix.Color(0, 30, 0),    // Dimmest Red
  matrix.Color(0, 15, 0)     // Almost Off
};

const int numShades = sizeof(redShades) / sizeof(redShades[0]);  // Calculate the number of shades

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
  matrix.setBrightness(25); // Adjust brightness if needed
  matrix.clear();
  matrix.show();
}

void loop() {
  beatHeart();
  delay(600);  // Delay to make the total beat cycle 1 second (60 BPM)
}

// Function to animate the heart with a smooth beating effect
void beatHeart() {
  // Beat in with increasing intensity
  for (int i = 0; i < numShades; i++) {
    matrix.clear();
    drawHeart(0, 0, redShades[i]);
    matrix.show();
    delay(50);  // Smooth transition between shades
  }
  
  // Beat out with decreasing intensity
  for (int i = numShades - 1; i >= 0; i--) {
    matrix.clear();
    drawHeart(0, 0, redShades[i]);
    matrix.show();
    delay(50);  // Smooth transition between shades
  }
}

// Function to draw the heart at the center of the matrix
void drawHeart(int offsetX, int offsetY, uint32_t color) {
  int centerX = (MATRIX_WIDTH - 8) / 2;  // Center the heart on the X axis
  int centerY = (MATRIX_HEIGHT - 8) / 2; // Center the heart on the Y axis

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      if (heartShape[y] & (1 << (7 - x))) {
        int drawX = x + centerX + offsetX;
        int drawY = y + centerY + offsetY;
        if (drawX >= 0 && drawX < MATRIX_WIDTH && drawY >= 0 && drawY < MATRIX_HEIGHT) {
          drawPixel(drawX, drawY, color);
        }
      }
    }
  }
}

// Function to draw a single pixel on the matrix
void drawPixel(int x, int y, uint32_t color) {
  int pixelIndex = y * MATRIX_WIDTH + x;
  matrix.setPixelColor(pixelIndex, color);
}
