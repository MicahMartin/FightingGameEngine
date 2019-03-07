#include <bitset>
#include "states/OpeningState.h"
#include "screens/OpeningScreen.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
  printf("opening state constructor\n");
  currentScreen = new OpeningScreen();
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
  if(vc->anyPressed()){
    printf("a button was pressed, transitioning to menu state \n");
    sm->pushState(&mainMenu);
  }
}

void OpeningState::update() {
}

void OpeningState::draw(SDL_Renderer* renderer) {
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  currentScreen->draw(renderer);
}
