#include "input/VirtualController.h"
#include <bitset>
#include <iostream>

VirtualController::Input VirtualController::getInputForString(std::string inputStr, bool faceRight){
  if (inputStr == "n") {
    return VirtualController::NOINPUT;
  } else if (inputStr == "u") {
    return VirtualController::UP;
  } else if (inputStr == "d") {
    return VirtualController::DOWN;
  } else if (inputStr == "f") {
    return faceRight ? VirtualController::RIGHT : VirtualController::LEFT;
  } else if (inputStr == "b") {
    return faceRight ? VirtualController::LEFT : VirtualController::RIGHT;
  }
};

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
  inputHistory.push_front(std::vector<uint16_t>());
}

bool VirtualController::isPressed(VirtualController::Input input) {
  if(input == NOINPUT && currentState == 0){
    std::cout << "NEUTRAL DEteCTED" << std::endl;
    return true;
  }
  return (input & currentState);
}

bool VirtualController::wasPressed(VirtualController::Input input) {
  for(uint16_t bytes : inputHistory.front()) {
    if(bytes & input){
      return true;
    }
  }
  return false;
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

