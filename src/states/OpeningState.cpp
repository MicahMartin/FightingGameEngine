#include <bitset>
#include "states/OpeningState.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
}

OpeningState::~OpeningState() { 
  printf("openingState destructor called\n");
}

void OpeningState::enter() {
  printf("Entered the opening state\n");
}

void OpeningState::exit() { 
  printf("leaving the opening state! \n");
}

void OpeningState::handleInput(StateManager* sm, VirtualController* vc) {
  if(vc->wasPressed(InputManager::DOWN)){
    printf("down was pressed, transitioning to menu state \n");
    sm->pushState(&mainMenu);
  }
}

void OpeningState::update() {
}

void OpeningState::draw() {
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  // currentScreen->draw();
}
