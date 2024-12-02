#include "level_four.hpp"
#include <Arduino.h>

static int currentRow = 0;
static int currentCol = 0;

void levelFourUpdate(GameState &gameState, CRGB leds[]) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= (gameState.interval / 4)) {
        gameState.previousMillis = currentMillis;

        // Reset the previous LED to its original color
        int previousRow = currentRow;
        int previousCol = currentCol;
        int previousLedIndex = gameState.ledPins[previousRow][previousCol];
        if (gameState.pinkStates[previousLedIndex]) {
            setLEDColor(gameState, leds, previousRow, previousCol, CRGB(255, 105, 180)); // Set to pink
        } else {
            setLEDColor(gameState, leds, previousRow, previousCol, CRGB(0, 0, 255)); // Set to blue
        }
        gameState.redStates[previousRow][previousCol] = false;

        // Update the column index
        currentCol++;
        if (currentCol >= COLS) {
            currentCol = 0;
            currentRow++;
            if (currentRow >= ROWS) {
                currentRow = 0;
            }
        }

        // Set the current LED to red
        setLEDColor(gameState, leds, currentRow, currentCol, CRGB(255, 0, 0)); // Set to red
        gameState.redStates[currentRow][currentCol] = true;
    }
}
