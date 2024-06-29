// src/level_one.cpp

#include "level_one.hpp"
#include <Arduino.h>

static int currentRedRow = 0;
static bool directionDown = true;

// void levelOneSetup(GameState &gameState, CRGB leds[]) {

//   // Set up LEDs
//   setAllBlue(leds);

//   gameState.levelCleared = false;

//   // Initialize LED states
//   for (int i = 0; i < NUM_LEDS; i++) {
//     gameState.purpleStates[i] = false;
//   }

//   setRandomPurple(gameState, leds);
// }

void setRowBlue(GameState &gameState, CRGB leds[], int row) {
  for (int col = 0; col < COLS; col++) {
    int ledIndex = gameState.ledPins[row][col];
    if (gameState.purpleStates[ledIndex]) {
      leds[ledIndex] = CRGB(255, 255, 255); // Set to purple if it was originally purple
    } else {
      leds[ledIndex] = CRGB(0, 0, 255); // Set to blue otherwise
    }
    gameState.redStates[row][col] = false;
  }
}

void moveToNextRow(GameState &gameState) {
  if (directionDown) {
    currentRedRow++;
    if (currentRedRow >= ROWS) {
      currentRedRow = ROWS - 1;
      directionDown = false; // Change direction to up
    }
  } else {
    currentRedRow--;
    if (currentRedRow < 0) {
      currentRedRow = 0;
      directionDown = true; // Change direction to down
    }
  }
}

void setRowRed(GameState &gameState, CRGB leds[], int row) {
  for (int col = 0; col < COLS; col++) {
    leds[gameState.ledPins[row][col]] = CRGB(255, 0, 0); // Set to red
    gameState.redStates[row][col] = true;
  }
}


void levelOneUpdate(GameState &gameState, CRGB leds[]) {
  unsigned long currentMillis = millis();
  if (currentMillis - gameState.previousMillis >= gameState.interval) {
    gameState.previousMillis = currentMillis;

    printScore(gameState);
    setRowBlue(gameState, leds, currentRedRow);
    moveToNextRow(gameState);
    setRowRed(gameState, leds, currentRedRow);
  }
}