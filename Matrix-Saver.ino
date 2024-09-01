#include <Adafruit_NeoPixel.h>

#define MATRIX_PIN 14
#define MATRIX_WIDTH 8
#define MATRIX_HEIGHT 8
#define NUMPIXELS (MATRIX_WIDTH * MATRIX_HEIGHT)

Adafruit_NeoPixel matrix = Adafruit_NeoPixel(NUMPIXELS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

struct Column {
  int position;  // Current position of the head of the column
  int length;    // Length of the column
  int speed;     // Speed of the column's movement
  unsigned long lastUpdateTime; // Time of the last movement update
};

Column columns[MATRIX_WIDTH];

void setup() {
  matrix.begin();
  matrix.setBrightness(20);  // Set to a dimmer brightness for the green "Matrix" effect
  matrix.clear();
  matrix.show();

  // Initialize the columns with random lengths and speeds
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    columns[i].position = random(-MATRIX_HEIGHT, 0);  // Start above the screen
    columns[i].length = random(4, MATRIX_HEIGHT + 1); // Length of the trailing effect
    columns[i].speed = random(100, 300);  // Speed of falling in milliseconds per step
    columns[i].lastUpdateTime = millis();
  }
}

void loop() {
  matrix.clear();

  // Update each column
  for (int i = 0; i < MATRIX_WIDTH; i++) {
    // Check if it's time to update the column based on its speed
    if (millis() - columns[i].lastUpdateTime >= columns[i].speed) {
      columns[i].position++;
      columns[i].lastUpdateTime = millis();  // Reset the update time

      // Reset the column if it has moved off the screen
      if (columns[i].position - columns[i].length > MATRIX_HEIGHT) {
        columns[i].position = random(-MATRIX_HEIGHT, 0);
        columns[i].length = random(4, MATRIX_HEIGHT + 1);
        columns[i].speed = random(100, 300);
      }
    }

    // Draw the column
    for (int j = 0; j < columns[i].length; j++) {
      int pos = columns[i].position - j;
      if (pos >= 0 && pos < MATRIX_HEIGHT) {
        uint32_t color = (j == 0) ? matrix.Color(255, 0, 0) : matrix.Color(25, 0, 0);  // Head is brighter green
        matrix.setPixelColor(pos * MATRIX_WIDTH + i, color);
      }
    }
  }

  matrix.show();
}
