// level_two.hpp
#pragma once

#include <FastLED.h>
#include "game_state.hpp"
#include "utilities.hpp"

struct GameState;

// Function prototypes
void levelTwoSetup(GameState &gameState, CRGB leds[]);
void levelTwoUpdate(GameState &gameState, CRGB leds[]);
void setRandomPurple(GameState &gameState, CRGB leds[]);
