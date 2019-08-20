#include <bitset>
#include "states/OpeningState.h"
#include "screens/OpeningScreen.h"
#include "input/InputManager.h"

OpeningState::OpeningState(){ 
  printf("opening state constructor\n");
  openingScreen = new OpeningScreen();
}

OpeningState::~OpeningState() { 
  printf("openingState destructor called\n");
}

void OpeningState::enter() {
  // initialize the menu state & opening screen
  printf("Entered the opening state\n");
  openingScreen->init();
}

void OpeningState::exit() { 
  // 
  printf("leaving the opening state! \n");
}

void OpeningState::pause() {
  printf("pausing the opening state\n");
}

void OpeningState::resume() { 
  printf("resuming the opening state! \n");
}

void OpeningState::handleInput(VirtualController* input) {
}

void OpeningState::update() {
  openingScreen->update();
}

void OpeningState::draw() {
  openingScreen->draw();
}
