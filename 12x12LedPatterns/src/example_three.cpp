#include "example_three.hpp"
#include <Arduino.h>

static bool setupLevel = true;
static int leftCol = 1; // Left red column
static int rightCol = COLS - 2; // Right red column
static int direction = 1; // 1 = inward, -1 = outward

void levelThreeUpdate(GameState &gameState, CRGB leds[]) {
    if (setupLevel) {
        setAllBlack(leds);
        setOuterGreen(gameState, leds);
        setRandomPink(gameState, leds);
        setupLevel = false;
    }

    // Update column positions
    if (direction == 1 && leftCol < COLS / 2 - 1) {
        leftCol++;
        rightCol--;
    } else if (direction == -1 && leftCol > 1) {
        leftCol--;
        rightCol++;
    } else {
        direction *= -1; // Reverse direction
    }
    clearAllLEDSToBlack(gameState, leds);

    // Update the positions of the red columns
    for (int row = 1; row < ROWS - 1; row++) {
        leds[gameState.ledPinsSnaked[row][leftCol]] = CRGB::Red;
        leds[gameState.ledPinsSnaked[row][rightCol]] = CRGB::Red;
    }
}
