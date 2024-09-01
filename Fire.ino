#include <Adafruit_NeoPixel.h>

#define MATRIX_PIN 14
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUMPIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, MATRIX_PIN, NEO_RGB + NEO_KHZ800);

int fireGrid[MATRIX_WIDTH][MATRIX_HEIGHT];

void setup() {
  matrix.begin();
  matrix.setBrightness(50); // Set brightness level
  matrix.clear();
  matrix.show();

  // Initialize the fire grid
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      fireGrid[x][y] = 0; // Start with all "off"
    }
  }
}

void loop() {
  // Step 1: Generate the fire at the bottom row
  generateFire();

  // Step 2: Propagate the fire upward
  propagateFire();

  // Step 3: Render the fire to the matrix
  renderFire();

  delay(100); // Control the speed of the fire animation
}

// Generate new fire at the bottom row
void generateFire() {
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    fireGrid[x][MATRIX_HEIGHT - 1] = random(0, 256); // Random intensity
  }
}

// Propagate the fire upward
void propagateFire() {
  for (int y = 0; y < MATRIX_HEIGHT - 1; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      int decay = random(0, 50); // Random decay factor
      int newIntensity = fireGrid[x][y + 1] - decay;
      fireGrid[x][y] = constrain(newIntensity, 0, 255); // Ensure within bounds
    }
  }
}

// Render the fire to the LED matrix
void renderFire() {
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      matrix.setPixelColor(y * MATRIX_WIDTH + x, getFireColor(fireGrid[x][y]));
    }
  }
  matrix.show();
}

// Map fire intensity to color (in RGB format)
uint32_t getFireColor(int intensity) {
  if (intensity > 200) {
    return matrix.Color(255, 255, 0); // Yellow (high intensity)
  } else if (intensity > 150) {
    return matrix.Color(255, 165, 0); // Orange
  } else if (intensity > 50) {
    return matrix.Color(255, 0, 0);   // Red
  } else {
    return matrix.Color(0, 0, 0);     // Black (off)
  }
}
