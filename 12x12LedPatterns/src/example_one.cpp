#include "example_one.hpp"
#include <Arduino.h>

static int currentQuadrant = 0; // 0, 1, 2, 3 equals upperleft, upperright, bottomleft, bottomright
static bool setupLevel = true;

// void setAllBlue(CRGB leds[]) {
//     for (int i = 0; i < NUM_LEDS; i++) {
//         leds[i] = CRGB(0, 0, 255);
//     }
// }

void setSomeGreen(GameState &gameState, CRGB leds[]) {
  for (int row = 0; row < ROWS; row++) {
    int ledIndex_1 = gameState.ledPinsSnaked[row][COLS/2-1];
    int ledIndex_2 = gameState.ledPinsSnaked[row][COLS/2];
    leds[ledIndex_1] = CRGB(0, 255, 0);
    leds[ledIndex_2] = CRGB(0, 255, 0);
    gameState.greenStates[row][COLS / 2 - 1] = true; // Track green LEDs
    gameState.greenStates[row][COLS / 2] = true; 
  }

  for (int col = 0; col < COLS; col++) {
    int ledIndex_1 = gameState.ledPinsSnaked[ROWS/2-1][col];
    int ledIndex_2 = gameState.ledPinsSnaked[ROWS/2][col];
    leds[ledIndex_1] = CRGB(0, 255, 0);
    leds[ledIndex_2] = CRGB(0, 255, 0);
    gameState.greenStates[ROWS/2-1][col] = true; // Track green LEDs
    gameState.greenStates[ROWS/2][col] = true; 
  }
}

void levelOneUpdate(GameState &gameState, CRGB leds[]) {
  if (setupLevel) 
  {
    setAllBlue(leds);
    setSomeGreen(gameState, leds);
    setRandomPink(gameState, leds);
    setupLevel = false;
  }
  unsigned long currentMillis = millis();
  if (currentMillis - gameState.previousMillis >= gameState.interval) {
    gameState.previousMillis = currentMillis;
    gameState.endGameCounter ++;
    checkNextLevel(gameState);
    currentQuadrant = (currentQuadrant + 1) % 4;
    if (currentQuadrant == 0) // upperleft
    {
      for (int row = 0; row < ROWS/2-1; row++) {
        for (int col = 0; col < COLS/2-1; col++) {
          if (!gameState.greenStates[row][col])
          {
            int ledIndex = gameState.ledPinsSnaked[row][col];
            leds[ledIndex] = CRGB(0, 255, 0);
          }
        }
      }
      for (int row = ROWS/2; row < ROWS; row++) {
        for (int col = 0; col < COLS/2-1; col++) {
          int ledIndex = gameState.ledPinsSnaked[row][col];
          if (!gameState.pinkStates[row][col]) {            
            leds[ledIndex] = CRGB(0, 0, 255); //set to Blue
          }
          else{
            leds[ledIndex] = CRGB(255, 105, 180); // Set to pink
          }
        }
      }
    }

    else if (currentQuadrant == 1) // upperright
    {
      for (int row = 0; row < ROWS/2-1; row++) {
        for (int col = COLS/2; col < COLS; col++) {
          if (!gameState.greenStates[row][col])
          {
            int ledIndex = gameState.ledPinsSnaked[row][col];
            leds[ledIndex] = CRGB(0, 255, 0);
          }
        }
      }
      for (int row = 0; row < ROWS/2-1; row++) {
        for (int col = 0; col < COLS/2-1; col++) {
          int ledIndex = gameState.ledPinsSnaked[row][col];
          if (!gameState.pinkStates[row][col]) {            
            leds[ledIndex] = CRGB(0, 0, 255); //set to Blue
          }
          else{
            leds[ledIndex] = CRGB(255, 105, 180); // Set to pink
          }
        }
      }
    }

    else if (currentQuadrant == 2) // lowerleft
    {
      for (int row = ROWS/2; row < ROWS; row++) {
        for (int col = 0; col < COLS/2-1; col++) {
          if (!gameState.greenStates[row][col])
          {
            int ledIndex = gameState.ledPinsSnaked[row][col];
            leds[ledIndex] = CRGB(0, 255, 0);
          }
        }
      }
      for (int row = 0; row < ROWS/2-1; row++) {
        for (int col = COLS/2; col < COLS; col++) {
          int ledIndex = gameState.ledPinsSnaked[row][col];
          if (!gameState.pinkStates[row][col]) {            
            leds[ledIndex] = CRGB(0, 0, 255); //set to Blue
          }
          else{
            leds[ledIndex] = CRGB(255, 105, 180); // Set to pink
          }
        }
      }
    }

    else if (currentQuadrant == 3) // lowerright
    {
      for (int row = ROWS/2; row < ROWS; row++) {
        for (int col = COLS/2; col < COLS; col++) {
          if (!gameState.greenStates[row][col])
          {
            int ledIndex = gameState.ledPinsSnaked[row][col];
            leds[ledIndex] = CRGB(0, 255, 0);
          }
        }
      }
      for (int row = ROWS/2; row < ROWS; row++) {
        for (int col = 0; col < COLS/2-1; col++) {
          int ledIndex = gameState.ledPinsSnaked[row][col];
          if (!gameState.pinkStates[row][col]) {            
            leds[ledIndex] = CRGB(0, 0, 255); //set to Blue
          }
          else{
            leds[ledIndex] = CRGB(255, 105, 180); // Set to pink
          }
        }
      }
    }
  }
}