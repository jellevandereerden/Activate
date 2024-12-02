#pragma once

#include <Arduino.h>

#define SENSOR_AMOUNT 15
#define LED_PIN 3
#define NUM_LEDS 15
#define THRESHOLD_FACTOR 50
#define ROWS 5
#define COLS 3

// Define the structure to hold the game state
struct GameState {
  bool uponNewLevel = true;
  int currentLevel = 1;
  int lives = 10;
  int score = 0;
  bool levelCleared = false;
  int pointPanels[5] = {};
  int ledPins[ROWS][COLS] = {
    {0, 5, 10},
    {1, 6, 11},
    {2, 7, 12},
    {3, 8, 13},
    {4, 9, 14}
  };
  int ledPinsSnaked[ROWS][COLS] = {
    {0, 9, 10},
    {1, 8, 11},
    {2, 7, 12},
    {3, 6, 13},
    {4, 5, 14}
  };
  bool redStates[ROWS][COLS] = {
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false}
  };
  bool pinkStates[NUM_LEDS] = {false};
  unsigned long previousMillis = 0;
  unsigned long previousMillisYellow = 0;
  unsigned long interval = 1000;
  unsigned long yellowinterval = 100;
  unsigned long lastLifeDecreaseTime = 0;
  const unsigned long lifeDecreaseCooldown = 5000;
  int s0 = 12;
  int s1 = 11;
  int s2 = 10;
  int s3 = 9;
  int SIG_pin = A0;
};