#include <bitset>
#include "states/MenuState.h"
#include "input/InputManager.h"

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

GameState* MenuState::handleInput(uint16_t inputBits) {

  if(inputBits == InputManager::DOWN){
    menuCounter == 2 ? menuCounter = 0 : menuCounter++;
    printf("%s is the mother fucking option niggah\n", menu[menuCounter].c_str());
  }
  if(inputBits == InputManager::UP){
    menuCounter == 0 ? menuCounter = 2 : menuCounter--;
    printf("%s is the mother fucking option niggah\n", menu[menuCounter].c_str());
  }
}

void MenuState::update(StateManager* stateManager){
}

void MenuState::draw(){ 
  //currentScreen->draw();
}