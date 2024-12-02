// src/level_one.cpp

#include "level_one.hpp"
#include <Arduino.h>

static int currentRedRow = 0;
static bool directionDown = true;

void setRowBlue(GameState &gameState, CRGB leds[], int row) {
  for (int col = 0; col < COLS; col++) {
    int ledIndex = gameState.ledPins[row][col];
    if (gameState.pinkStates[ledIndex]) {
      leds[ledIndex] = CRGB(255, 105, 180); // Set to pink if it was originally pink
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

    // printScore(gameState);
    setRowBlue(gameState, leds, currentRedRow);
    moveToNextRow(gameState);
    setRowRed(gameState, leds, currentRedRow);
  }
}