#include <bitset>
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
}

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
    internalState = 2;
  }
  // update internal state
}

void OpeningState::update(StateManager* stateManager) {
  if (internalState == 2) {
    stateManager->pushState(new MenuState());
  }
}

void OpeningState::draw() {
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  // currentScreen->draw();
}
