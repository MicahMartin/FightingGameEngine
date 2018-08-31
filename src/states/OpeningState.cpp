#include <bitset>
#include <iostream>
#include "input/VirtualController.h"
#include "states/OpeningState.h"
#include "states/MenuState.h"
#include "Game.h"

OpeningState::OpeningState(Game* gameP) : game(gameP) {}

OpeningState::~OpeningState() { 

  printf("openingState destructor called");
}

void OpeningState::enter() {

  printf("Entered the opening state");
}

void OpeningState::exit() { 

  printf("leaving the opening state! \n");
}

void OpeningState::pause() { }

void OpeningState::resume() { }

GameState* OpeningState::handleInput(uint16_t inputBits){

  printf("in opening, heres the current bitset for input %s\n", std::bitset<16>(inputBits).to_string().c_str());

  uint8_t stickState = (inputBits & 0x0f);
  switch (stickState) {
    case VirtualController::DOWNRIGHT:
      return new MenuState(game);
    break;
    default:
      return NULL;
  }
}

void OpeningState::update() {
  printf("updating the opening state\n");

}

void OpeningState::draw() {

  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
}

//void OpeningState::setCurrentScene(Scene* scene){

// currentScene = scene; 
//}

//Scene* OpeningState::getCurrentScene(){

//  return currentScene;
//}
