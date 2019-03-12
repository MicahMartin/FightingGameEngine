#include <bitset>
#include "states/OpeningState.h"
#include "screens/OpeningScreen.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
  printf("opening state constructor\n");
  openingScreen.init();
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

void OpeningState::pause() {
  printf("pausing the opening state\n");
}

void OpeningState::resume() { 
  printf("resuming the opening state! \n");
}

void OpeningState::handleInput(StateManager* sm, VirtualController* vc) {
  if(vc->wasPressed(InputManager::DOWN)){
    printf("down was pressed\n");
    sm->pushState(&mainMenu);
  }
}

void OpeningState::update() {
  openingScreen.update();
}

void OpeningState::draw() {
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  openingScreen.draw();
}
