#include <bitset>
#include <iostream>
#include "input/VirtualController.h"
#include "states/MenuState.h"
#include "states/OpeningState.h"
#include "Game.h"

MenuState::MenuState(StateCollection* stateCollection) : stateCollection(stateCollection){
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
}

GameState* MenuState::handleInput(uint16_t inputBits) {
  uint8_t stickState = (inputBits & 0x0f);
  printf("in menu, the current bitset for input %s\n", std::bitset<16>(inputBits).to_string().c_str());

  switch (stickState) {
    case VirtualController::DOWNLEFT:
      printf("woohoo\n");
      stateCollection->swapLast();
    break;
      
    default:
      return NULL;
  }
}

void MenuState::update(){
  printf("updating the menu state\n"); 
}

void MenuState::draw(){ 
  //currentScreen->draw();
}
