#include <bitset>
#include "states/MenuState.h"

MenuState::MenuState(){
  // set game pointer
  // get virtual controller pointer(s) from game pointer
}

MenuState::~MenuState(){ }

void MenuState::enter() {
  // setCurrentScreen(new OpeningScreen);
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
  //cleanup
  delete this;
}

GameState* MenuState::handleInput(VirtualController* vc) {
  return this;
}

void MenuState::update(){
}

void MenuState::draw(){ 
  //currentScreen->draw();
}
