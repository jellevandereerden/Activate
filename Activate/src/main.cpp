#include <FastLED.h>

#define SENSOR_AMOUNT 15
#define LED_PIN 3
#define NUM_LEDS 15
#define THRESHOLD_FACTOR 50
#define ROWS 5
#define COLS 3

CRGB leds[NUM_LEDS];

// Define the structure to hold the game state
struct GameState {
  int currentLevel;
  int score = 0;
  bool levelCleared = false;
  int pointPanels[5] = {};
  int ledPins[ROWS][COLS] = {
    {0, 5, 10},
    {1, 6, 11},
    {2, 7, 12},
    {3, 8, 13},
    {4, 9, 14}
  };
  bool redStates[ROWS][COLS] = {
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false},
    {false, false, false}
  };
  bool purpleStates[NUM_LEDS] = {false};
  unsigned long previousMillis = 0;
  unsigned long interval = 1000;
  int currentRedRow = 0; // Start with the first row
  bool directionDown = true; // Direction of the moving bar (true = down, false = up)
  int s0 = 12;
  int s1 = 11;
  int s2 = 10;
  int s3 = 9;
  int SIG_pin = A0;
};

GameState gameState; // Instance of the game state

// Function prototypes
void setupGame();
void updateGame();
void checkLevelCompletion();
void setAllBlue();
void flashAllGreen();
void setRowBlue(int row);
void moveToNextRow();
void setRowRed(int row);
void setRandomPurple();
std::pair<int, int> findValue(int value);
bool pressure_detection(uint8_t analog_pin);
int readMux(int channel);

void setup() {
  pinMode(gameState.s0, OUTPUT);
  pinMode(gameState.s1, OUTPUT);
  pinMode(gameState.s2, OUTPUT);
  pinMode(gameState.s3, OUTPUT);
  pinMode(gameState.SIG_pin, INPUT);

  Serial.begin(9600);
  randomSeed(analogRead(0) + analogRead(1) + analogRead(2)); // Seed the random number generator with more random inputs

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  setAllBlue();
  setRandomPurple();
  FastLED.show();

  gameState.currentLevel = 1;
  setupGame(); // Initialize the game state
}

void loop() {
  if(!gameState.levelCleared)
  {
    updateGame(); // Update the game state each loop
  }
  else
  {
    flashAllGreen();
  }
  FastLED.show();
}

void setupGame() {
  gameState.score = 0;
  gameState.levelCleared = false;

  // Initialize LED states
  for (int i = 0; i < NUM_LEDS; i++) {
    gameState.purpleStates[i] = false;
  }
  
  gameState.currentRedRow = 0;
  gameState.directionDown = true;

  // Reset point panels and set them to random positions
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

  // Select first 5 as point panels
  for (size_t i = 0; i < sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0]); i++) {
    gameState.pointPanels[i] = numbers[i];
    leds[numbers[i]] = CRGB(255, 255, 255); // Set to purple
    gameState.purpleStates[numbers[i]] = true;
  }
}

void updateGame() {
  unsigned long currentMillis = millis();
  if (currentMillis - gameState.previousMillis >= gameState.interval) {
    gameState.previousMillis = currentMillis;

    Serial.print("SCORE = ");
    Serial.println(gameState.score);
    Serial.println("-----------------");

    setRowBlue(gameState.currentRedRow);
    moveToNextRow();
    setRowRed(gameState.currentRedRow);
  }

  // Check pressure detection and update LEDs accordingly
  for (int panel = 0; panel < SENSOR_AMOUNT; panel++) {
    if (pressure_detection(panel)) {
      std::pair<int, int> result = findValue(panel);
      if (!gameState.redStates[result.first][result.second]) {
        for (size_t k = 0; k < sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0]); k++) {
          if (panel == gameState.pointPanels[k]) {
            gameState.pointPanels[k] = -1;
            gameState.score += 1;
            gameState.purpleStates[panel] = false;
          }
          leds[panel] = CRGB(0, 255, 0); // Turn on LED into green
        }
      } else {
        leds[panel] = CRGB(255, 255, 0); // Turn on LED into yellow (TODO: Implement blinking)
      }
    } else {
      std::pair<int, int> result = findValue(panel);
      if (gameState.redStates[result.first][result.second]) {
        leds[panel] = CRGB(255, 0, 0); // If it was red, keep it red
      } else {
        if (gameState.purpleStates[panel]) {
          leds[panel] = CRGB(255, 255, 255); // Restore to purple if originally purple
        } else {
          leds[panel] = CRGB(0, 0, 255); // Otherwise restore to blue
        }
      }
    }
  }
  checkLevelCompletion(); // Check if the level is completed
}

void checkLevelCompletion() {
  if (gameState.score == sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0])) {
    Serial.println("----------------------------------------------");
    Serial.println("GEWONNNNENNNNNNNNNNNNNN");
    Serial.println("----------------------------------------------");
    gameState.levelCleared = true;
  }
}

void setAllBlue() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 255);
  }
}

void flashAllGreen() {
  static unsigned long previousMillis = 0;
  static bool toggle = false;
  const unsigned long interval = 500; // Interval in milliseconds (0.5 seconds)

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    toggle = !toggle;

    for (int i = 0; i < NUM_LEDS; i++) {
      if (toggle) {
        leds[i] = CRGB(0, 255, 0); // Green color
      } else {
        leds[i] = CRGB(0, 0, 0); // Black (off)
      }
    }
  }
}

void setRowBlue(int row) {
  for (int col = 0; col < COLS; col++) {
    int ledIndex = gameState.ledPins[row][col];
    if (gameState.purpleStates[ledIndex]) {
      leds[ledIndex] = CRGB(255, 255, 255); // Set to purple if it was originally purple
    } else {
      leds[ledIndex] = CRGB(0, 0, 255); // Set to blue otherwise
    }
    gameState.redStates[row][col] = false;
  }
}

void moveToNextRow() {
  if (gameState.directionDown) {
    gameState.currentRedRow++;
    if (gameState.currentRedRow >= ROWS) {
      gameState.currentRedRow = ROWS - 1;
      gameState.directionDown = false; // Change direction to up
    }
  } else {
    gameState.currentRedRow--;
    if (gameState.currentRedRow < 0) {
      gameState.currentRedRow = 0;
      gameState.directionDown = true; // Change direction to down
    }
  }
}

void setRowRed(int row) {
  for (int col = 0; col < COLS; col++) {
    leds[gameState.ledPins[row][col]] = CRGB(255, 0, 0); // Set to red
    gameState.redStates[row][col] = true;
  }
}

void setRandomPurple() {
  // Initialize an array with LED indices
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

std::pair<int, int> findValue(int value) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (gameState.ledPins[i][j] == value) {
        return std::make_pair(i, j); // Return row and column as a pair
      }
    }
  }
  return std::make_pair(-1, -1); // Return (-1, -1) if value is not found
}

bool pressure_detection(uint8_t analog_pin) {
  int pressure = readMux(analog_pin);
  return (abs(pressure) > THRESHOLD_FACTOR);
}

int readMux(int channel) {
  int controlPin[] = {gameState.s0, gameState.s1, gameState.s2, gameState.s3};
  int muxChannel[16][4]={
    {0,0,0,0}, {1,0,0,0}, {0,1,0,0}, {1,1,0,0}, 
    {0,0,1,0}, {1,0,1,0}, {0,1,1,0}, {1,1,1,0}, 
    {0,0,0,1}, {1,0,0,1}, {0,1,0,1}, {1,1,0,1}, 
    {0,0,1,1}, {1,0,1,1}, {0,1,1,1}, {1,1,1,1} 
  }; 
  
  // Loop through the 4 sig 
  for(int i = 0; i < 4; i ++) { 
    digitalWrite(controlPin[i], muxChannel[channel][i]); 
  } 

  // Read the value at the SIG pin 
  return analogRead(gameState.SIG_pin); // Return the value 
}
