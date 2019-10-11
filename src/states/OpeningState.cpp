#include <bitset>
#include "states/OpeningState.h"
#include "screens/OpeningScreen.h"
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
  mainMenu = new MenuState();
  openingScreen = new OpeningScreen();
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
  if(input->wasPressed(DOWN, 0)){
    printf("down was pressed\n");
    stateManager->pushState(mainMenu);
  }
}

void OpeningState::update() {
  openingScreen->update();
}

void OpeningState::draw() {
  openingScreen->draw();
}
