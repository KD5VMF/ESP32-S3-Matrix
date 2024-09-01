#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL 14
#define NUMPIXELS    64  // 8x8 matrix
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define RGB_BRIGHTNESS 50  // Brightness level (0-255)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Paddle structure
struct Paddle {
  int y;             // y position (top of the paddle)
  uint32_t color;    // Color of the paddle
};

Paddle paddle1, paddle2;

// Ball structure
struct Ball {
  int x, y;          // Position of the ball
  int dx, dy;        // Direction of movement
  uint32_t color;    // Color of the ball
};

Ball ball;

void setup() {
  pixels.begin(); // Initialize the NeoPixel library
  pixels.setBrightness(RGB_BRIGHTNESS); // Set brightness for all LEDs

  // Initialize paddles
  paddle1.y = 3;  // Start in the middle of the left side
  paddle1.color = pixels.Color(255, 255, 0); // Yellow

  paddle2.y = 3;  // Start in the middle of the right side
  paddle2.color = pixels.Color(255, 0, 0); // Red

  // Initialize ball in the center with a random direction
  ball.x = 4;
  ball.y = 3;
  ball.dx = random(0, 2) * 2 - 1; // Random direction: -1 or 1
  ball.dy = random(0, 2) * 2 - 1; // Random direction: -1 or 1
  ball.color = pixels.Color(0, 0, 255); // Blue
}

void loop() {
  // Clear the matrix
  pixels.clear();

  // AI for Player 1 (Yellow) - Left Side
  if (ball.y < paddle1.y) {
    paddle1.y--;
    if (paddle1.y < 0) paddle1.y = 0;
  } else if (ball.y > paddle1.y + 1) {
    paddle1.y++;
    if (paddle1.y > MATRIX_HEIGHT - 2) paddle1.y = MATRIX_HEIGHT - 2;
  }

  // AI for Player 2 (Red) - Right Side
  if (ball.y < paddle2.y) {
    paddle2.y--;
    if (paddle2.y < 0) paddle2.y = 0;
  } else if (ball.y > paddle2.y + 1) {
    paddle2.y++;
    if (paddle2.y > MATRIX_HEIGHT - 2) paddle2.y = MATRIX_HEIGHT - 2;
  }

  // Move the ball
  ball.x += ball.dx;
  ball.y += ball.dy;

  // Ball collision with top or bottom wall
  if (ball.y <= 0 || ball.y >= MATRIX_HEIGHT - 1) {
    ball.dy = -ball.dy; // Reverse y-direction
  }

  // Ball collision with paddles
  if (ball.x == 1 && ball.y >= paddle1.y && ball.y <= paddle1.y + 1) {
    ball.dx = -ball.dx; // Reverse x-direction
    ball.x = 2; // Prevents sticking to the paddle
  } else if (ball.x == MATRIX_WIDTH - 2 && ball.y >= paddle2.y && ball.y <= paddle2.y + 1) {
    ball.dx = -ball.dx; // Reverse x-direction
    ball.x = MATRIX_WIDTH - 3; // Prevents sticking to the paddle
  }

  // Check if the ball went out of bounds (reset the ball in this case)
  if (ball.x < 0 || ball.x >= MATRIX_WIDTH) {
    ball.x = 4;
    ball.y = 3;
    ball.dx = random(0, 2) * 2 - 1;
    ball.dy = random(0, 2) * 2 - 1;
  }

  // Draw paddles
  for (int i = 0; i < 2; i++) {
    // Player 1 (Yellow) - Left Side
    pixels.setPixelColor((paddle1.y + i) * MATRIX_WIDTH, paddle1.color);
    
    // Player 2 (Red) - Right Side
    pixels.setPixelColor((paddle2.y + i) * MATRIX_WIDTH + (MATRIX_WIDTH - 1), paddle2.color);
  }

  // Draw the ball
  int pixelIndex = ball.y * MATRIX_WIDTH + ball.x;
  pixels.setPixelColor(pixelIndex, ball.color);

  // Show the updated matrix
  pixels.show();
  delay(150);  // Adjust to change the speed of the game
}
