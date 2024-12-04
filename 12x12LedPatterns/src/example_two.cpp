#include "example_two.hpp"
#include <Arduino.h>

static bool setupLevel = true;
static int direction = 0;

void levelTwoUpdate(GameState &gameState, CRGB leds[]) {
    if (setupLevel) 
    {
        setAllBlack(leds);
        setOuterGreen(gameState, leds);
        setRandomPink(gameState, leds);
        setupLevel = false;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= gameState.interval) {
        gameState.previousMillis = currentMillis;
        gameState.endGameCounter ++;
        checkNextLevel(gameState);
        clearAllLEDSToBlack(gameState, leds);
        
        for (int diag = 0; diag < (ROWS - 2) + (COLS - 2) - 1; diag += direction) { //Change the 2 if more safe panels, because now I only use the outer ring for safe green
            for (int row = 1; row < (ROWS - 1); row++) {
                for (int col = 1; col < (COLS - 1); col++) {
                    leds[gameState.ledPinsSnaked[row][col]] = (row + col == diag)? CRGB::Red : CRGB::Black;
                }
            }
        }
        direction *= -1;
    }
}