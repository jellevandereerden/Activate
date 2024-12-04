#include "example_five.hpp"
#include <Arduino.h>

static bool setupLevel = true;
static int currentRowUpperLeft = 0;
static int currentColUpperLeft = 0;

void setGreenBlockMiddle(GameState &gameState, CRGB leds[]) {
    for (int row = 4; row < (ROWS - 4); row++) {
        for (int col = 4; col < (COLS - 4); col++) {
            leds[gameState.ledPinsSnaked[row][col]] = CRGB::Green;
            gameState.greenStates[row][col] = true;
        }
    }
}

void moveRedBlock(GameState &gameState, CRGB leds[]) {
    for (int row = currentRowUpperLeft; row < (currentRowUpperLeft + 4); row++) {
        for (int col = currentColUpperLeft; col < (currentColUpperLeft + 4); col++) {
            leds[gameState.ledPinsSnaked[row][col]] = CRGB::Red;
        }
    }
}

void updateDirectionsRedBlock() {
    if (currentColUpperLeft < 7 && currentRowUpperLeft == 0) {
        currentColUpperLeft++;
    }
    else if (currentRowUpperLeft < 7 && currentColUpperLeft == 7) {
        currentRowUpperLeft++;
    }
    else if (currentColUpperLeft > 0 && currentRowUpperLeft == 7) {
        currentColUpperLeft--;
    }
    else if (currentRowUpperLeft > 0 && currentColUpperLeft == 0) {
        currentRowUpperLeft--;
    }
}

void levelFiveUpdate(GameState &gameState, CRGB leds[]) {
    if (setupLevel) {
        setAllBlack(leds);
        setGreenBlockMiddle(gameState, leds);
        setRandomPink(gameState, leds);
        setupLevel = false;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= gameState.interval) {
        gameState.previousMillis = currentMillis;
        gameState.endGameCounter ++;
        checkNextLevel(gameState);
        clearAllLEDSToBlack(gameState, leds);
        moveRedBlock(gameState, leds);
        updateDirectionsRedBlock();
    }
}