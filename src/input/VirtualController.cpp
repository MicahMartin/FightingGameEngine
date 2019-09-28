#include "input/VirtualController.h"
#include <bitset>
#include <iostream>
#include <nlohmann/json.hpp>

std::map<std::string, VirtualController::Input(*)(bool)> VirtualController::inputMap = {
  {"n", [](bool faceRight){return VirtualController::NOINPUT;}},
  {"u", [](bool faceRight){return VirtualController::UP;}},
  {"d", [](bool faceRight){return VirtualController::DOWN;}},
  {"f", [](bool faceRight){return faceRight ? VirtualController::RIGHT:VirtualController::LEFT;}},
  {"uf", [](bool faceRight){return faceRight ? VirtualController::UPRIGHT:VirtualController::UPLEFT;}},
  {"df", [](bool faceRight){return faceRight ? VirtualController::DOWNRIGHT:VirtualController::DOWNLEFT;}},
  {"b", [](bool faceRight){return faceRight ? VirtualController::LEFT:VirtualController::RIGHT;}},
  {"ub", [](bool faceRight){return faceRight ? VirtualController::UPLEFT:VirtualController::UPRIGHT;}},
  {"db", [](bool faceRight){return faceRight ? VirtualController::DOWNLEFT:VirtualController::DOWNRIGHT;}},
  {"lp", [](bool faceRight){return VirtualController::LP;}},
  {"lk", [](bool faceRight){return VirtualController::LK;}},
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
  if(input == NOINPUT && (currentState == 0 || ((currentState & LEFT) && (currentState & RIGHT)))) {
    return true;
  }
  return (input == (currentState & 0x0F));
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

