#include "level_three.hpp"
#include <Arduino.h>

static int currentRedIndex = 0;
static bool directionForward = true;

void levelThreeUpdate(GameState &gameState, CRGB leds[]) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= gameState.interval) {
        gameState.previousMillis = currentMillis;

        printScore(gameState);
        Serial.println("LOLLLEOEOEOELELELE");

        // Set previous LED position to blue and update gameState.redStates
        leds[gameState.ledPins[currentRedIndex][0]] = CRGB(0, 0, 255);
        gameState.redStates[currentRedIndex][0] = false;

        // Move to the next LED index in the back and forth pattern
        if (directionForward) {
            currentRedIndex = 1;  // Move forward to index 1
        } else {
            currentRedIndex = 0;  // Move backward to index 0
        }

        // Set current LED position to red and update gameState.redStates
        leds[gameState.ledPins[currentRedIndex][0]] = CRGB(255, 0, 0);
        gameState.redStates[currentRedIndex][0] = true;

        // Toggle direction for the next update
        directionForward = !directionForward;
    }
}
