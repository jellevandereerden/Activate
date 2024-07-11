#include <FastLED.h>
#include "game_state.hpp"
#include "utilities.hpp"

#include "level_one.hpp"
#include "level_two.hpp"
#include "level_three.hpp"

CRGB leds[NUM_LEDS];

GameState gameState;

void flashBlueAndOff();
void handleSerialInput();
void resetGame();
void gameLogic();
void checkLevelCompletion();
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
  randomSeed(analogRead(0) + analogRead(1) + analogRead(2));

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {

  handleSerialInput();

  if(!gameState.levelCleared)
  {
    if(gameState.uponNewLevel){
      levelSetup(gameState, leds);
      gameState.uponNewLevel = false;
    }
    if(gameState.currentLevel == 1) {
      if(gameState.uponNewLevel){
        gameState.uponNewLevel = false;
      }
      levelOneUpdate(gameState, leds);
    }
    else if(gameState.currentLevel == 2) {
      if(gameState.uponNewLevel){
        gameState.uponNewLevel = false;
      }
      levelTwoUpdate(gameState, leds);
    }
    else if(gameState.currentLevel == 3) {
      if(gameState.uponNewLevel){
        gameState.uponNewLevel = false;
      }
      levelThreeUpdate(gameState, leds);
    }
    gameLogic();
    FastLED.show();
  }
  else
  {
    clearLevel(leds, gameState);
  }
}

void handleSerialInput() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command.length() > 0) { // Check if the command is not empty
      Serial.println(command);
    }
    if (command == "RESTART") {
      flashBlueAndOff();
      resetGame(); // Call the function to flash blue and off
    }
    else if (command.startsWith("GOTO LEVEL")) {
      // Extract the level number from the command
      int level = -1; // Initialize with an invalid level number
      int indexOfLevel = command.lastIndexOf(' ');
      if (indexOfLevel != -1 && indexOfLevel + 1 < static_cast<int>(command.length())) {
        String levelString = command.substring(indexOfLevel + 1);
        level = levelString.toInt(); // Convert string to integer
      }

      // Validate the level number and proceed accordingly
      if (level >= 1 && level <= 3) {
        flashBlueAndOff();
        gameState.currentLevel = level;
        gameState.uponNewLevel = true;
        gameState.levelCleared = false; // Assume level is cleared to trigger setup
      } else {
        Serial.println("Invalid level number!");
      }
    }
    // Handle other commands as needed
    else {
      Serial.println("Unknown command!");
    }
  }
}

void flashBlueAndOff() {
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < NUM_LEDS; ++j) {
      leds[j] = CRGB::Blue;
    }
    FastLED.show();
    delay(500);

    for (int j = 0; j < NUM_LEDS; ++j) {
      leds[j] = CRGB::Black;
    }
    FastLED.show();
    delay(500);
  }
}

void resetGame() {
  gameState.currentLevel = 1;
  gameState.levelCleared = false;
  gameState.score = 0;
  gameState.uponNewLevel = true;
  Serial.print("SCORE: ");
  Serial.println(gameState.score);
  Serial.print("LEVEL: ");
  Serial.println(gameState.currentLevel);
}

void gameLogic(){
  for (int panel = 0; panel < SENSOR_AMOUNT; panel++) {
    if (pressure_detection(panel)) {
      // Serial.print("PRESSURE DETECTED, PANEL: ");
      // Serial.println(panel);
      std::pair<int, int> result = findValue(panel);
      if (!gameState.redStates[result.first][result.second]) {
        for (size_t k = 0; k < sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0]); k++) {
          if (panel == gameState.pointPanels[k]) {
            gameState.pointPanels[k] = -1;
            gameState.score += 1;
            Serial.print("SCORE: ");
            Serial.println(gameState.score);
            gameState.whiteStates[panel] = false;
          }
          leds[panel] = CRGB(0, 255, 0); // Turn on LED into green
        }
      } else {
        flashYellow(gameState, leds, panel);
        }
    } else {
      std::pair<int, int> result = findValue(panel);
      if (gameState.redStates[result.first][result.second]) {
        leds[panel] = CRGB(255, 0, 0); // If it was red, keep it red
      } else {
        if (gameState.whiteStates[panel]) {
          leds[panel] = CRGB(255, 255, 255); // Restore to white if originally white
        } else {
          leds[panel] = CRGB(0, 0, 255); // Otherwise restore to blue
        }
      }
    }
  }
  checkLevelCompletion(); // Check if the level is completed
}

void checkLevelCompletion() {
  if (gameState.score == static_cast<int>(sizeof(gameState.pointPanels) / sizeof(gameState.pointPanels[0])) * gameState.currentLevel) {
    Serial.println("----------------------------------------------");
    Serial.print("CONGRATZ, YOUR GOING TO LEVEL: ");
    Serial.print(gameState.currentLevel + 1);
    Serial.println("----------------------------------------------");
    gameState.levelCleared = true;
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
  // Serial.println("Pressure = ");
  // Serial.println(pressure);
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
