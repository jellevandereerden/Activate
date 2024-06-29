// // level_two.cpp

// #include "level_two.hpp"
// #include <Arduino.h>

// void levelTwoSetup(GameState &gameState, CRGB leds[]) {

//   // Set up LEDs
//   setAllBlue(leds);

//   gameState.levelCleared = false;

//   // Initialize LED states
//   for (int i = 0; i < NUM_LEDS; i++) {
//     gameState.purpleStates[i] = false;
//   }

//   gameState.currentRedCol = 0;
//   gameState.directionRight = true;

//   setRandomPurple(gameState, leds);
// }

// void setColBlue(GameState &gameState, CRGB leds[], int col) {
//   for (int row = 0; row < ROWS; row++) {
//     int ledIndex = gameState.ledPins[row][col];
//     if (gameState.purpleStates[ledIndex]) {
//       leds[ledIndex] = CRGB(255, 255, 255); // Set to purple if it was originally purple
//     } else {
//       leds[ledIndex] = CRGB(0, 0, 255); // Set to blue otherwise
//     }
//     gameState.redStates[row][col] = false;
//   }
// }

// void moveToNextCol(GameState &gameState) {
//   if (gameState.directionRight) {
//     gameState.currentRedCol++;
//     if (gameState.currentRedCol >= COLS) {
//       gameState.currentRedCol = COLS - 1;
//       gameState.directionRight = false; // Change direction to left
//     }
//   } else {
//     gameState.currentRedCol--;
//     if (gameState.currentRedCol < 0) {
//       gameState.currentRedCol = 0;
//       gameState.directionRight = true; // Change direction to right
//     }
//   }
// }

// void setColRed(GameState &gameState, CRGB leds[], int col) {
//   for (int row = 0; row < ROWS; row++) {
//     leds[gameState.ledPins[row][col]] = CRGB(255, 0, 0); // Set to red
//     gameState.redStates[row][col] = true;
//   }
// }

// void levelTwoUpdate(GameState &gameState, CRGB leds[]) {
//   unsigned long currentMillis = millis();
//   if (currentMillis - gameState.previousMillis >= gameState.interval) {
//     gameState.previousMillis = currentMillis;

//     // Update game state
//     Serial.print("Current score = ");
//     Serial.println(gameState.score);
//     Serial.println("-----------------");

//     setColBlue(gameState, leds, gameState.currentRedCol);
//     moveToNextCol(gameState);
//     setColRed(gameState, leds, gameState.currentRedCol);
//   }
// }
