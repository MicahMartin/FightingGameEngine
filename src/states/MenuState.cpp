#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

MenuState::MenuState(StateManager* stateManager) : stateManager(stateManager) {
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

GameState* MenuState::handleInput(uint16_t inputBits) {
  if(inputBits == InputManager::DOWNLEFT){
    printf("downLEFT BABY\n");
    stateManager->popState();
  }
}

void MenuState::update(){
}

void MenuState::draw(){ 
  //currentScreen->draw();
}
