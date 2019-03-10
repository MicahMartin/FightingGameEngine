#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(){
  printf("menuState constructor\n");
}

MenuState::~MenuState(){ }

void MenuState::enter() {
  printf("entered the menu state\n");
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
}

void MenuState::pause() {
  printf("pausing the menu state\n");
}

void MenuState::resume() { 
  printf("resuming the menu state! \n");
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
