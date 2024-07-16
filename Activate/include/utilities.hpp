#pragma once

#include "game_state.hpp"
#include <FastLED.h>

void setAllBlue(CRGB leds[]);
void setRandomWhite(GameState &gameState, CRGB leds[]);
void flashAllGreen(CRGB leds[]);
void turnOffLeds(CRGB leds[]);
void printScore(GameState &gameState);
void levelSetup(GameState &gameState, CRGB leds[]);
void flashYellow(GameState &gameState, CRGB leds[], int panel);
void clearLevel(CRGB leds[], GameState &gameState);
void setLEDColor(GameState &gameState, CRGB leds[], int row, int col, CRGB color);