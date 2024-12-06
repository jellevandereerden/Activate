#include <iostream>
#include <vector>
#include <FastLED.h> // Include the FastLED library for LED control

#define NUM_LEDS 144 // Adjust based on your LED strip size
CRGB leds[NUM_LEDS]; // Array to store LED states

int matrixWidth = 12; // Width of the matrix

// Function to calculate the LED index based on row and column
int calculateLEDIndex(int row, int col) {
    return row * matrixWidth + col; // Adjust this if your layout differs
}

int main() {
    // Initialize LED strip
    FastLED.addLeds<WS2812, 6, GRB>(leds, NUM_LEDS);

    // Input data: Each vector represents one saved state
    std::vector<std::vector<std::pair<int, int>>> redTileStates = {
        {{0, 1}, {1, 1}, {1, 2}},
        {{3, 5}, {6, 5}},
        {{0, 1}, {1, 1}, {1, 2}, {6, 6}}
    };

    // Process each state
    for (const auto& state : redTileStates) {
        // Turn LEDs red for each index in the current state
        for (const auto& [row, col] : state) {
            int ledIndex = calculateLEDIndex(row, col);
            if (ledIndex < NUM_LEDS) { // Ensure index is within bounds
                leds[ledIndex] = CRGB::Red;
                redStates[row, col] = true; //RESET THEM BACK TO FALSE EACH TIME
            }
        }

        // Show the updated LED colors
        FastLED.show();

        // Simulate some delay to observe changes (optional)
        std::cout << "Processed a state.\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // Keep LEDs on (in case of single run)
    while (true) {
        FastLED.show();
    }

    return 0;
}
