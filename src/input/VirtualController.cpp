#include "input/VirtualController.h"

VirtualController::VirtualController() { }
VirtualController::~VirtualController() { } 

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
  return (currentState& 0x0f);
}


void VirtualController::onNotify(const char* eventName) { }

