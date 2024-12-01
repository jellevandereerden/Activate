// level_two.cpp

#include "level_two.hpp"
#include <Arduino.h>

static int currentRedCol = 0;
static bool directionRight = true;

void setColBlue(GameState &gameState, CRGB leds[], int col) {
  for (int row = 0; row < ROWS; row++) {
    int ledIndex = gameState.ledPins[row][col];
    if (gameState.whiteStates[ledIndex]) {
      leds[ledIndex] = CRGB(255, 255, 255); // Set to white if it was originally white
    } else {
      leds[ledIndex] = CRGB(0, 0, 255); // Set to blue otherwise
    }
    gameState.redStates[row][col] = false;
  }
}

void moveToNextCol(GameState &gameState) {
  if (directionRight) {
    currentRedCol++;
    if (currentRedCol >= COLS) {
      currentRedCol = COLS - 1;
      directionRight = false; // Change direction to left
    }
  } else {
    currentRedCol--;
    if (currentRedCol < 0) {
      currentRedCol = 0;
      directionRight = true; // Change direction to right
    }
  }
}

void setColRed(GameState &gameState, CRGB leds[], int col) {
  for (int row = 0; row < ROWS; row++) {
    leds[gameState.ledPins[row][col]] = CRGB(255, 0, 0); // Set to red
    gameState.redStates[row][col] = true;
  }
}

void levelTwoUpdate(GameState &gameState, CRGB leds[]) {
  unsigned long currentMillis = millis();
  if (currentMillis - gameState.previousMillis >= gameState.interval) {
    gameState.previousMillis = currentMillis;

    // printScore(gameState);
    setColBlue(gameState, leds, currentRedCol);
    moveToNextCol(gameState);
    setColRed(gameState, leds, currentRedCol);
  }
}
