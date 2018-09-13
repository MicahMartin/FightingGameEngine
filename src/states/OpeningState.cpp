#include <bitset>
#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "Game.h"

OpeningState::OpeningState(Game* game) : game(game) { }

OpeningState::~OpeningState() { 
  printf("openingState destructor called");
}

void OpeningState::enter() {
  printf("Entered the opening state");
  setCurrentScreen(new OpeningScreen(game->getGraphics()));
}

void OpeningState::exit() { 
  printf("leaving the opening state! \n");
}

void OpeningState::pause() { }

void OpeningState::resume() { }

GameState* OpeningState::handleInput(uint16_t inputBits) {
  uint8_t stickState = (inputBits & 0x0f);
  switch (stickState) {
    case VirtualController::DOWNRIGHT:
      return new MenuState(game);
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
  currentScreen->draw();
}
