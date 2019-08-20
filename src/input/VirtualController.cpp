#include "input/VirtualController.h"
#include <bitset>
#include <iostream>

VirtualController::VirtualController() { 
  inputHistory.set_capacity(60);
}

VirtualController::~VirtualController() { } 

void VirtualController::setBit(uint16_t bit) {
  currentState |= bit;
  inputHistory.front().push_back(currentState);
  printStickState();
}

void VirtualController::clearBit(uint16_t bit) {
  currentState &= ~bit;
  inputHistory.front().push_back(currentState);
  printStickState();
}

void VirtualController::update() {
  inputHistory.push_front(std::vector<uint16_t>{0});
}

void VirtualController::printStickState(){
  std::bitset<4> stickState(currentState);
  std::cout << stickState << std::endl;
}
uint16_t VirtualController::getState() {
  return currentState;
}

uint8_t VirtualController::getStickState() {
  return (currentState& 0x0f);
}



void VirtualController::onNotify(const char* eventName) { }

