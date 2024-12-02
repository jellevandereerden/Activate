// example_one.hpp
#pragma once

#include <FastLED.h>
// #include "game_state.hpp"
#include "utilities.hpp"

void setSomeGreen(GameState &gameState, CRGB leds[]);
void setRandomPink(GameState &gameState, CRGB leds[]);
void levelOneUpdate(GameState &gameState, CRGB leds[]);
