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
  // Refactor input history
  std::cout << std::bitset<16>(stickState)<< std::endl;
}

void VirtualController::setBit(uint16_t bit) {
  currentState |= bit;
}

void VirtualController::clearBit(uint16_t bit) {
  currentState &= ~bit;
}

uint16_t VirtualController::getState() {
  return currentState;
}

uint8_t VirtualController::getStickState() {
  return stickState;
}

int VirtualController::getInputHistorySize() { 
  return 0;
};

void VirtualController::onNotify(const char* eventName) { }

