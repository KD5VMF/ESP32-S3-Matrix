#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL 14
#define NUMPIXELS    64  // 8x8 matrix
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define RGB_BRIGHTNESS 50  // Brightness level (0-255)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

struct Ball {
  int x, y;             // Position of the ball
  int dx, dy;           // Direction of movement
  uint32_t color;       // Color of the ball
};

Ball ball;

void setup() {
  pixels.begin(); // Initialize the NeoPixel library
  pixels.setBrightness(RGB_BRIGHTNESS); // Set brightness for all LEDs

  // Initialize the ball at a random position with a random direction
  ball.x = random(0, MATRIX_WIDTH);
  ball.y = random(0, MATRIX_HEIGHT);
  ball.dx = random(0, 2) * 2 - 1; // Random direction: -1 or 1
  ball.dy = random(0, 2) * 2 - 1; // Random direction: -1 or 1

  // Set the ball color to solid blue
  ball.color = pixels.Color(0, 0, 255); // Solid blue color
}

void loop() {
  // Clear the matrix
  pixels.clear();

  // Move the ball
  ball.x += ball.dx;
  ball.y += ball.dy;

  // Check for wall collisions and change direction
  if (ball.x <= 0 || ball.x >= MATRIX_WIDTH - 1) {
    ball.dx = -ball.dx; // Reverse the x-direction
  }
  if (ball.y <= 0 || ball.y >= MATRIX_HEIGHT - 1) {
    ball.dy = -ball.dy; // Reverse the y-direction
  }

  // Set the pixel for the ball
  int pixelIndex = ball.y * MATRIX_WIDTH + ball.x;
  pixels.setPixelColor(pixelIndex, ball.color);

  // Show the updated matrix
  pixels.show();
  delay(100);  // Adjust to change the speed of the ball
}
