#include "utilities.hpp"

void setAllBlack(CRGB leds[]) {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
}

std::pair<int, int> findValue(GameState &gameState, int value) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (gameState.ledPinsSnaked[i][j] == value) {
        return std::make_pair(i, j); // Return row and column as a pair
      }
    }
  }
  return std::make_pair(-1, -1); // Return (-1, -1) if value is not found
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
    int randomCounter = 0;
    int pinkCounter = 0;
    while (pinkCounter < 15)
    {
      std::pair<int, int> result = findValue(gameState, numbers[randomCounter]);
      if (!gameState.greenStates[result.first][result.second]) {
        gameState.pointPanels[pinkCounter] = numbers[randomCounter];
        leds[numbers[randomCounter]] = CRGB(255, 105, 180); // Set to pink
        gameState.pinkStates[result.first][result.second] = true;
        pinkCounter++;
      }
    randomCounter++;
    }
}

void checkNextLevel(GameState &gameState) {
    if (gameState.endGameCounter > 15) {
        gameState.currentLevel++;
        gameState.endGameCounter = 0;
        gameState.resetGreenStates();
        gameState.resetPinkStates();
    }
}

void setOuterGreen(GameState &gameState, CRGB leds []) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (row == 0 || row == ROWS - 1 || col == 0 || col == COLS - 1) {
                leds[gameState.ledPinsSnaked[row][col]] = CRGB::Green;
                gameState.greenStates[row][col] = true;
            }
        }
    }
}

void clearAllLEDSToBlack(GameState &gameState, CRGB leds[]) {
    for (int row = 1; row < (ROWS - 1); row++) {
        for (int col = 1; col < (COLS - 1); col++) {
            leds[gameState.ledPinsSnaked[row][col]] =! gameState.pinkStates[row][col] ? CRGB::Black : CRGB::Pink;
        }
    }
}
    
