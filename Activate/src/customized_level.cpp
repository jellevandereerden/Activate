#include "customized_level.hpp"
#include <Arduino.h>

static int currentFrame = 0;

std::vector<std::vector<std::pair<int, int>>> redTileStates = {
    {{0, 1}, {1, 1}, {1, 2}},
    {{3, 5}, {6, 5}},
    {{0, 1}, {1, 1}, {1, 2}, {6, 6}}
};

std::vector<std::vector<std::pair<int, int>>> greenTileStates = {
    {{9, 1}},
    {{3, 4}, {8, 5}, {8, 1}, {4, 2}, {14, 1}, {12, 2}},
    {{0, 2}, {1, 6}, {3, 2}}
};

void levelCustomizedUpdate(GameState &gameState, CRGB leds[]) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= gameState.interval) {
        gameState.previousMillis = currentMillis;
        for (const auto& [row, col] : redTileStates[currentFrame]) {
            setLEDColor(gameState, leds, row, col, CRGB(255, 0, 0));
            // TODO: Update redstates
        }

        for (const auto& [row, col] : greenTileStates[currentFrame]) {
            setLEDColor(gameState, leds, row, col, CRGB(0, 255, 0));
            // TODO: Update greenstates

        }
        (currentFrame += 1) % greenTileStates.size();
    }
}