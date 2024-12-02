#pragma once

#include <Arduino.h>

constexpr int SENSOR_AMOUNT = 144;
constexpr int LED_PIN = 3;
constexpr int NUM_LEDS = 144;
constexpr int THRESHOLD_FACTOR = 50;
constexpr int ROWS = 12;
constexpr int COLS = 12;

// Define the structure to hold the game state
struct GameState {
  bool uponNewLevel = true;
  int currentLevel = 1;
  int lives = 10;
  int score = 0;
  bool levelCleared = false;
  int pointPanels[15] = {};
  int ledPinsSnaked[ROWS][COLS] = {
    {0, 23, 24, 47, 48, 71, 72, 95, 96, 119, 120, 143},
    {1, 22, 25, 46, 49, 70, 73, 94, 97, 118, 121, 142},
    {2, 21, 26, 45, 50, 69, 74, 93, 98, 117, 122, 141},
    {3, 20, 27, 44, 51, 68, 75, 92, 99, 116, 123, 140},
    {4, 19, 28, 43, 52, 67, 76, 91, 100, 115, 124, 139},
    {5, 18, 29, 42, 53, 66, 77, 90, 101, 114, 125, 138},
    {6, 17, 30, 41, 54, 65, 78, 89, 102, 113, 126, 137},
    {7, 16, 31, 40, 55, 64, 79, 88, 103, 112, 127, 136},
    {8, 15, 32, 39, 56, 63, 80, 87, 104, 111, 128, 135},
    {9, 14, 33, 38, 57, 62, 81, 86, 105, 110, 129, 134},
    {10, 13, 34, 37, 58, 61, 82, 85, 106, 109, 130, 133},
    {11, 12, 35, 36, 59, 60, 83, 84, 107, 108, 131, 132},
  };
  bool greenStates[ROWS][COLS] = {};
  bool pinkStates[ROWS][COLS] = {};
  unsigned long previousMillis = 0;
  unsigned long previousMillisEndGame = 0;
  unsigned long interval = 1000;
  int endGameCounter = 0;
  int s0 = 12;
  int s1 = 11;
  int s2 = 10;
  int s3 = 9;
  int SIG_pin = A0;

  GameState() {
    for (int row = 0; row < ROWS; row++) {
      for (int col = 0; col < COLS; col++) {
        greenStates[row][col] = false;
        pinkStates[row][col] = false;
      }
    }
  }
};