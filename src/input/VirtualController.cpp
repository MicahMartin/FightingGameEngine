#include <bitset>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include "input/VirtualController.h"
#include "input/InputManager.h"



VirtualController::VirtualController() { }
VirtualController::~VirtualController() { } 
void VirtualController::init() { }

void VirtualController::update(uint16_t inputBits) {
  // printf("currentState %d \n", currentState);
  // std::cout << "Heres the current byte of input" << std::bitset<16>(inputByte) << std::endl;
  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  // set lowest nibble to stick state
  currentState = inputBits;
  stickState = (inputBits & 0x0f);
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
