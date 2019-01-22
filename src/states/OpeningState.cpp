#include <bitset>
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "input/InputManager.h"

OpeningState::OpeningState(StateManager* stateManager) : stateManager(stateManager) { }

OpeningState::~OpeningState() { 
  printf("openingState destructor called");
}

void OpeningState::enter() {
  printf("Entered the opening state");
}

void OpeningState::exit() { 
  printf("leaving the opening state! \n");
  //cleanup
  delete this;
}

GameState* OpeningState::handleInput(uint16_t inputBits) {
  if(inputBits == InputManager::DOWNRIGHT){
    printf("downright fierce\n");
    stateManager->pushState(new MenuState(stateManager));
  }
}

void OpeningState::update() {
}

void OpeningState::draw() {
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  // currentScreen->draw();
}
