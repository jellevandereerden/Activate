#include <FastLED.h>
#include "game_state.hpp"
#include "utilities.hpp"

#include "level_one.hpp"
#include "level_two.hpp"
#include "level_three.hpp"

CRGB leds[NUM_LEDS];

GameState gameState;

void gameLogic();
void checkLevelCompletion();
// void flashAllGreen();
// void turnOfLeds();
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
    flashAllGreen(leds);
    turnOffLeds(leds);
    gameState.uponNewLevel = true;
    gameState.currentLevel++;
    gameState.levelCleared = false;
  }
}

void gameLogic(){
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
        flassYellow(gameState, leds, panel);
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
