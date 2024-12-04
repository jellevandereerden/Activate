#include <FastLED.h>
#include "game_state.hpp"
// #include "utilities.hpp"
#include "example_one.hpp"
#include "example_two.hpp"
#include "example_three.hpp"
#include "example_four.hpp"
#include "example_five.hpp"

CRGB leds[NUM_LEDS];

GameState gameState;

void setup() {
  pinMode(gameState.s0, OUTPUT);
  pinMode(gameState.s1, OUTPUT);
  pinMode(gameState.s2, OUTPUT);
  pinMode(gameState.s3, OUTPUT);
  pinMode(gameState.SIG_pin, INPUT);

  Serial.begin(9600);
  randomSeed(analogRead(0) + analogRead(1) + analogRead(2));

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
  if(gameState.currentLevel == 1) {
    levelOneUpdate(gameState, leds);
  }
  else if(gameState.currentLevel == 2) {
    levelTwoUpdate(gameState, leds);
  }
  else if(gameState.currentLevel == 3) {
    levelThreeUpdate(gameState, leds);
  }
  else if(gameState.currentLevel == 4) {
    levelFourUpdate(gameState, leds);
  }
  else if(gameState.currentLevel == 5) {
    levelFiveUpdate(gameState, leds);
  }
  FastLED.show();
}