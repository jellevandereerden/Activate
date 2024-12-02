#pragma once

#include "game_state.hpp"
#include <FastLED.h>

void setAllBlue(CRGB leds[]);
std::pair<int, int> findValue(GameState &gameState, int value);
void setRandomPink(GameState &gameState, CRGB leds[]);
void checkNextLevel(GameState &gameState);

