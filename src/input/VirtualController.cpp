#include <bitset>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "input/VirtualController.h"
#include "input/InputManager.h"



VirtualController::VirtualController() { }
VirtualController::~VirtualController() { } 
void VirtualController::init() { }

void VirtualController::update() {
  // set lowest nibble to stick state
  stickState = (currentState& 0x0f);
  inputHistory.push_back(std::vector<uint16_t>());
  counter++;
}

void VirtualController::setBit(uint16_t bit) {
  currentState |= bit;
  printf("counter: %d\n", counter);
  inputHistory.at(counter).push_back(currentState);
}

void VirtualController::clearBit(uint16_t bit) {
  currentState &= ~bit;
  printf("counter: %d\n", counter);
  //inputHistory.at(counter).push_back(currentState);
}

void VirtualController::printLastFewFrames() {
  for (int i = 0; i < inputHistory.size(); ++i) {
    if (inputHistory[i].size() > 0) {
      for (auto input : inputHistory[i]) {
        printf("Frame:%d stickState: %s\n", i, std::bitset<16>(input).to_string().c_str());
      }
    }
  }
  printf("input history size: %d\n", (int)inputHistory.size());

}

uint16_t VirtualController::getState() {
  return currentState;
}

uint8_t VirtualController::getStickState() {
  return stickState;
}

void VirtualController::onNotify(const char* eventName) { }

int VirtualController::getInputHistorySize() { 
  return 0;
};
