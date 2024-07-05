#pragma once

#include "game_state.hpp"
#include <FastLED.h>

void setAllBlue(CRGB leds[]);
void setRandomPurple(GameState &gameState, CRGB leds[]);
void flashAllGreen(CRGB leds[]);
void turnOffLeds(CRGB leds[]);
void printScore(GameState &gameState);
void levelSetup(GameState &gameState, CRGB leds[]);
void flashYellow(GameState &gameState, CRGB leds[], int panel);