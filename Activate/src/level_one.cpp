// src/level_one.cpp

#include "level_one.hpp"
#include <Arduino.h>

static int currentRedRow = 0;
static bool directionDown = true;

void setColBlue(GameState &gameState, CRGB leds[], int row) {
  for (int col = 0; col < COLS; col++) {
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

void setColRed(GameState &gameState, CRGB leds[], int row) {
  for (int col = 0; col < COLS; col++) {
    leds[gameState.ledPins[row][col]] = CRGB(255, 0, 0); // Set to red
    gameState.redStates[row][col] = true;
  }
}


void levelOneUpdate(GameState &gameState, CRGB leds[]) {
  unsigned long currentMillis = millis();
  if (currentMillis - gameState.previousMillis >= gameState.interval) {
    gameState.previousMillis = currentMillis;

    // printScore(gameState);
    setColBlue(gameState, leds, currentRedRow);
    moveToNextCol(gameState);
    setColRed(gameState, leds, currentRedRow);
  }
}