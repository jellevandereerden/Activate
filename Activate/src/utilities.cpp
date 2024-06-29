#include "utilities.hpp"

void setAllBlue(CRGB leds[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 255);
    }
}

void setRandomPurple(GameState &gameState, CRGB leds[]) {
    int numbers[NUM_LEDS];
    for (int i = 0; i < NUM_LEDS; i++) {
        numbers[i] = i;
    }

    // Shuffle the array
    for (int i = (NUM_LEDS - 1); i > 0; i--) {
        int j = random(i + 1);
        int temp = numbers[i];
        numbers[i] = numbers[j];
        numbers[j] = temp;
    }

    // Set the first 5 LEDs to purple
    for (size_t i = 0; i < sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0]); i++) {
        gameState.pointPanels[i] = numbers[i];
        leds[numbers[i]] = CRGB(255, 255, 255); // Set to purple
        gameState.purpleStates[numbers[i]] = true;
    }
}

void flashAllGreen(CRGB leds[]) {
    static bool toggle = false;
    for (int times = 0; times < 10; times++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (toggle) {
                leds[i] = CRGB(0, 255, 0); // Green color
            } else {
                leds[i] = CRGB(0, 0, 0); // Black (off)
            }
        }
        FastLED.show();
        toggle = !toggle;
        delay(500);
    }
}

void turnOffLeds(CRGB leds[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 0);
    }
    FastLED.show();
    delay(5000);
}

void printScore(GameState &gameState) {
    Serial.print("Current score = ");
    Serial.println(gameState.score);
    Serial.println("-----------------");
}

void levelSetup(GameState &gameState, CRGB leds[]) {

  // Set up LEDs
  setAllBlue(leds);

  gameState.levelCleared = false;

  // Initialize LED states
  for (int i = 0; i < NUM_LEDS; i++) {
    gameState.purpleStates[i] = false;
  }

  setRandomPurple(gameState, leds);
}

void flassYellow(GameState &gameState, CRGB leds[], int panel) {
    unsigned long currentMillis = millis();
    if (currentMillis - gameState.previousMillis >= gameState.yellowinterval) {
        gameState.previousMillis = currentMillis;

        if (leds[panel] == CRGB::Yellow) {
        leds[panel] = CRGB::Black; // Turn off if currently yellow
        } else {
        leds[panel] = CRGB::Yellow; // Turn yellow if currently off
        }
    }
}