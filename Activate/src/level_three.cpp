#include "level_three.hpp"
#include <Arduino.h>

// static int currentRedIndex = 0;
static bool directionRightUp = true;
static int ledIndex;
static int multiply;
static int counter;

void levelThreeUpdate(GameState &gameState, CRGB leds[]) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= gameState.interval) {
        gameState.previousMillis = currentMillis;

        printScore(gameState);
        Serial.println("LOLLLEOEOEOELELELE");

        if (directionRightUp){
            multiply = 1;
        }
        else {
            multiply = -1;
        }

        for(int i = 0; i < ROWS - 1; i++) {
            int index = 0;
            while(index <= i) {
                ledIndex = gameState.ledPins[i-index][index];
                leds[ledIndex] = CRGB(255, 0, 0);
                gameState.ledPins[i-index][index] = true;
            }
            counter ++;
        }

        for(int i = 0; i < ((COLS - 1) - counter); i++) {
            for(int rows = 0; rows < ROWS; rows++) {
                ledIndex = gameState.ledPins[0+rows][3+i-rows];
                leds[ledIndex] = CRGB(255, 0, 0);
            }
        }

        
    }
}
