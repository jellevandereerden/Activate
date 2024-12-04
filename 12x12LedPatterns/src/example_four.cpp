#include "example_four.hpp"
#include <Arduino.h>

static int ledPinsSnakeTiles[38][2] = {
    {22, 25},
    {21, 26},
    {20, 27},
    {19, 28},
    {18, 29},
    {17, 30},
    {16, 31},
    {15, 32},
    {14, 33},
    {13, 33},
    {33, 34},
    {37, 38},
    {57, 58},
    {61, 62},
    {62, 82},
    {62, 81},
    {63, 80},
    {64, 79},
    {65, 78},
    {66, 77},
    {67, 76},
    {68, 75},
    {69, 74},
    {70, 74},
    {73, 74},
    {93, 94},
    {97, 98},
    {117, 118},
    {121, 117},
    {122, 117},
    {123, 116},
    {124, 115},
    {125, 114},
    {126, 113},
    {127, 112},
    {128, 111},
    {129, 110},
    {130, 109},
};

static bool setupLevel = true;
static int updateCount = 0;
static int currentLedIndexRed = 0;
static int size = sizeof(ledPinsSnakeTiles) / sizeof(ledPinsSnakeTiles[0]);
static int currentLedIndexBlack = (currentLedIndexRed - 5) % size;

void setExtraFixedRed(GameState &gameState, CRGB leds[]) { //This changes drastically if the 12x12 size or green thickness size changes
    for (int row = 2; row < ROWS; row++) {
        leds[gameState.ledPinsSnaked[row][3]] = CRGB::Red;
        gameState.greenStates[row][3] = true;
        leds[gameState.ledPinsSnaked[row][4]] = CRGB::Red;
        gameState.greenStates[row][4] = true;
    }
    for (int row = 0; row < (ROWS - 2); row++) {
        leds[gameState.ledPinsSnaked[row][7]] = CRGB::Red;
        gameState.greenStates[row][7] = true;
        leds[gameState.ledPinsSnaked[row][8]] = CRGB::Red;
        gameState.greenStates[row][8] = true;
    }
}

void levelFourUpdate(GameState &gameState, CRGB leds[]) {
    if (setupLevel) 
    {
        setAllBlack(leds);
        setOuterGreen(gameState, leds);
        setExtraFixedRed(gameState, leds);
        setRandomPink(gameState, leds);
        setupLevel = false;
    }
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= (gameState.interval)/5) {
        gameState.previousMillis = currentMillis;
        updateCount++;
        if (updateCount >= 5) {
            gameState.endGameCounter++; 
            updateCount = 0; 
            checkNextLevel(gameState);
        }

        leds[ledPinsSnakeTiles[currentLedIndexRed][0]] = CRGB::Red;
        leds[ledPinsSnakeTiles[currentLedIndexRed][1]] = CRGB::Red;
        leds[ledPinsSnakeTiles[currentLedIndexBlack][0]] =! gameState.pinkStates[currentLedIndexBlack][0] ? CRGB::Black : CRGB::Pink;
        leds[ledPinsSnakeTiles[currentLedIndexBlack][1]] =! gameState.pinkStates[currentLedIndexBlack][1] ? CRGB::Black : CRGB::Pink;
        currentLedIndexRed = (currentLedIndexRed + 1) % size;
        currentLedIndexBlack = (currentLedIndexBlack + 1) % size;
    }
}