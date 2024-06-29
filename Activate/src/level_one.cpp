// src/level_one.cpp

#include "level_one.hpp"
#include <Arduino.h>

void levelOneSetup(GameState &gameState, CRGB leds[]) {

  // Set up LEDs
  setAllBlue(leds);

  gameState.score = 0;
  gameState.levelCleared = false;

  // Initialize LED states
  for (int i = 0; i < NUM_LEDS; i++) {
    gameState.purpleStates[i] = false;
  }
  
  gameState.currentRedRow = 0;
  gameState.directionDown = true;

  setRandomPurple(gameState, leds);
}

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
  if (gameState.directionDown) {
    gameState.currentRedRow++;
    if (gameState.currentRedRow >= ROWS) {
      gameState.currentRedRow = ROWS - 1;
      gameState.directionDown = false; // Change direction to up
    }
  } else {
    gameState.currentRedRow--;
    if (gameState.currentRedRow < 0) {
      gameState.currentRedRow = 0;
      gameState.directionDown = true; // Change direction to down
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

    // Update game state
    Serial.print("Current score = ");
    Serial.println(gameState.score);
    Serial.println("-----------------");

    setRowBlue(gameState, leds, gameState.currentRedRow);
    moveToNextRow(gameState);
    setRowRed(gameState, leds, gameState.currentRedRow);
  }
}

void setAllBlue(CRGB leds[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 255);
  }
}

void setRandomPurple(GameState &gameState, CRGB leds[]) {
  int numbers[NUM_LEDS];
  for (int i = 0; i < NUM_LEDS; i++) {
    numbers[i] = i;
  }

  // Shuffle the array
  for (int i = (NUM_LEDS - 1); i > 0; i--) {
    int j = random(i + 1);
    int temp = numbers[i];
    numbers[i] = numbers[j];
    numbers[j] = temp;
  }

  // Set the first 5 LEDs to purple
  for (size_t i = 0; i < sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0]); i++) {
    gameState.pointPanels[i] = numbers[i];
    leds[numbers[i]] = CRGB(255, 255, 255); // Set to purple
    gameState.purpleStates[numbers[i]] = true;
  }
}