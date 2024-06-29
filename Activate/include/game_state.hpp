#pragma once

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
  bool redStates[ROWS][COLS] = {
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false}
  };
  bool purpleStates[NUM_LEDS] = {false};
  unsigned long previousMillis = 0;
  unsigned long interval = 1000;
  int currentRedRow = 0; // Start with the first row
  bool directionDown = true; // Direction of the moving bar (true = down, false = up)
  int s0 = 12;
  int s1 = 11;
  int s2 = 10;
  int s3 = 9;
  int SIG_pin = A0;
};