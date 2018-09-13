#include <bitset>
#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "Game.h"

OpeningState::OpeningState(StateCollection* stateCollection) : stateCollection(stateCollection) { }

OpeningState::~OpeningState() { 
  printf("openingState destructor called");
}

void OpeningState::enter() {
  printf("Entered the opening state");
}

void OpeningState::exit() { 
  printf("leaving the opening state! \n");
  stateCollection->popState();
}

GameState* OpeningState::handleInput(uint16_t inputBits) {
  uint8_t stickState = (inputBits & 0x0f);
  switch (stickState) {
    case VirtualController::DOWNRIGHT:
      // Menu state is behind this state, we are ok to swap.
      stateCollection->swapLast();
    break;

    default:
      return NULL;
  }
}

void OpeningState::update() {
  //printf("updating the opening state\n");
}

void OpeningState::draw() {
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  // currentScreen->draw();
}
