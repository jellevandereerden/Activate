// level_one.h
#pragma once

#include <FastLED.h>
#include "game_state.hpp"
#include "utilities.hpp"

struct GameState;

// Function prototypes
void levelOneSetup(GameState &gameState, CRGB leds[]);
void levelOneUpdate(GameState &gameState, CRGB leds[]);
void setRandomPurple(GameState &gameState, CRGB leds[]);
