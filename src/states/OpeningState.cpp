#include <bitset>
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
  printf("opening state constructor\n");
}

OpeningState::~OpeningState() { 
  printf("openingState destructor called\n");
}

void OpeningState::enter() {
  // initialize the menu state & opening screen
  printf("Entered the opening state\n");
}

void OpeningState::exit() { 
}

void OpeningState::pause() {
  printf("pausing the opening state\n");
}

void OpeningState::resume() { 
  printf("resuming the opening state! \n");
}

void OpeningState::handleInput() {
  VirtualController* input = inputManager->getVirtualController(0);
  if(input->wasPressed(DOWN)){
    printf("down was pressed\n");
    stateManager->pushState(new MenuState());
  }
}

void OpeningState::update() {
  openingScreen.update();
}

void OpeningState::draw() {
  openingScreen.draw();
}
