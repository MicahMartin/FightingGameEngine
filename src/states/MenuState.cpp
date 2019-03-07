#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  // set game pointer
  // get virtual controller pointer(s) from game pointer
}

MenuState::~MenuState(){ }

void MenuState::enter() {
  printf("entered the menu state\n");
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
}

void MenuState::handleInput(StateManager* sm, VirtualController* vc) {
  if(vc->wasPressed(InputManager::DOWN)){
    printf("up was pressed, transitioning back to opening state \n");
    // return to title screen
    sm->popState();
  }
}

void MenuState::update(){
}

void MenuState::draw(){ 
  //currentScreen->draw();
}
