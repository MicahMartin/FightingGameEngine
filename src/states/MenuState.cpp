#include <bitset>
#include <iostream>
#include "input/VirtualController.h"
#include "states/MenuState.h"
#include "states/OpeningState.h"
#include "Game.h"

MenuState::MenuState(Game* game) : game(game) {
  // set game pointer
  // get virtual controller pointer(s) from game pointer
}

MenuState::~MenuState(){ }

void MenuState::enter() {
  // setCurrentScene(new OpeningScene);
}

void MenuState::exit() { 
  printf("leaving the menu state! \n");
  //cleanup
}

void MenuState::pause() { }

void MenuState::resume() { }

GameState* MenuState::handleInput(uint16_t inputBits) {
  uint8_t stickState = (inputBits & 0x0f);
  printf("in menu, the current bitset for input %s\n", std::bitset<16>(inputBits).to_string().c_str());

  switch (stickState) {
    case VirtualController::DOWNLEFT:
      return new OpeningState(game);
    break;
      
    default:
      return NULL;
  }
}

void MenuState::update(){
  printf("updating the menu state\n"); 
}

void MenuState::draw(){ 
  //currentScene->draw();
}


void MenuState::setCurrentScene(Scene* scene){
 currentScene = scene; 
}

Scene* MenuState::getCurrentScene(){
  return currentScene;
}
