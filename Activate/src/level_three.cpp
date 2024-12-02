#include "level_three.hpp"
#include <Arduino.h>

int redIndexes[ROWS*COLS][2] = {
    {1, 1}, {2, 1}, {3, 1}, 
    {4, 1}, {4, 2}, {3, 2}, 
    {2, 2}, {1, 2}, {0, 2},
    {0, 1}, {0, 0}, {1, 0},
    {2, 0}, {3, 0}, {4, 0}
};

static int currentRedIndex = 0;
static bool directionForward = true;

void levelThreeUpdate(GameState &gameState, CRGB leds[]) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= (gameState.interval / 4)) {
        gameState.previousMillis = currentMillis;

        // Reset the previous LED to its original color
        int previousRow = redIndexes[currentRedIndex][0];
        int previousCol = redIndexes[currentRedIndex][1];
        int previousLedIndex = gameState.ledPins[previousRow][previousCol];
        if (gameState.pinkStates[previousLedIndex]) {
            setLEDColor(gameState, leds, previousRow, previousCol, CRGB(255, 105, 180)); // Set to pink
        } else {
            setLEDColor(gameState, leds, previousRow, previousCol, CRGB(0, 0, 255)); // Set to blue
        }
        gameState.redStates[previousRow][previousCol] = false;

        // Update the index
        if (directionForward) {
            currentRedIndex++;
            if (currentRedIndex >= ROWS * COLS) {
                currentRedIndex = ROWS * COLS - 1;
                directionForward = false; // Change direction to backward
            }
        } else {
            currentRedIndex--;
            if (currentRedIndex < 0) {
                currentRedIndex = 0;
                directionForward = true; // Change direction to forward
            }
        }

        // Set the current LED to red
        int currentRow = redIndexes[currentRedIndex][0];
        int currentCol = redIndexes[currentRedIndex][1];
        setLEDColor(gameState, leds, currentRow, currentCol, CRGB(255, 0, 0)); // Set to red
        gameState.redStates[currentRow][currentCol] = true;
    }
}
