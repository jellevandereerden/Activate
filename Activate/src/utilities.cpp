#include "utilities.hpp"

void setAllBlue(CRGB leds[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0, 0, 255);
    }
}

void setRandomPink(GameState &gameState, CRGB leds[]) {
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

    // Set the first 5 LEDs to pink
    for (size_t i = 0; i < sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0]); i++) {
        gameState.pointPanels[i] = numbers[i];
        leds[numbers[i]] = CRGB(255, 105, 180); // Set to pink
        gameState.pinkStates[numbers[i]] = true;
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

void finishedEntireGame(CRGB leds[]) {
    static bool toggle = false;
    while(true) {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (toggle) {
                leds[i] = CRGB(0, 255, 0); // Green color
            } else {
                leds[i] = CRGB(0, 0, 0); // Black (off)
            }
        }
        FastLED.show();
        toggle = !toggle;
        delay(50);
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
    gameState.pinkStates[i] = false;
  }

  setRandomPink(gameState, leds);
}

void flashYellow(GameState &gameState, CRGB leds[], int panel) {
    unsigned long currentMillisYellow = millis();
    if (currentMillisYellow - gameState.previousMillisYellow >= gameState.yellowinterval) {
        Serial.println("FAULT");
        gameState.previousMillisYellow = currentMillisYellow;
        if (leds[panel] == CRGB::Yellow) {
        leds[panel] = CRGB::Black; // Turn off if currently yellow
        } else {
        leds[panel] = CRGB::Yellow; // Turn yellow if currently off
        }
    }
}

void clearLevel(CRGB leds[], GameState &gameState){
  flashAllGreen(leds);
  turnOffLeds(leds);
  gameState.uponNewLevel = true;
  gameState.currentLevel++;
  gameState.levelCleared = false;
  Serial.print("LEVEL: ");
  Serial.println(gameState.currentLevel);
}

void setLEDColor(GameState &gameState, CRGB leds[], int row, int col, CRGB color) {
    int ledIndex = gameState.ledPinsSnaked[row][col];
    leds[ledIndex] = color;
}

void turnOffGame(CRGB leds[]) {
  // Assuming you have a function to set all LEDs to black
  setAllLEDsToBlack(leds);
  while (true) {
    // Stay in this loop indefinitely
    delay(1000);
  }
}

// Function to set all LEDs to black
void setAllLEDsToBlack(CRGB leds[]) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }
  FastLED.show();
}