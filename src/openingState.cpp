#include "openingState.h"
#include "menuState.h"
#include "game.h"
#include "virtualController.h"
#include <bitset>
#include <iostream>

OpeningState::OpeningState(Game* gameP){
  // set game pointer
  game = gameP;
  // get virtual controller pointer(s) from game pointer
  p1Controller = game->getInputManager()->getVirtualController();
}

OpeningState::~OpeningState(){}

void OpeningState::update(){
  uint8_t stickState = p1Controller->getState();
  printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
  if(stickState == VirtualController::DOWNRIGHT){
    printf("Stick is in down state");
    game->changeState(new MenuState(game));
    return;
  }
  draw();
}
void OpeningState::draw(){
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
}

void OpeningState::enter() {
  printf("Entered the opening state");
  // setCurrentScene(new OpeningScene);
}

void OpeningState::exit() { 

  printf("leaving the opening state! \n");
  // cleanup
}

void OpeningState::pause() { }

void OpeningState::resume() { }

//void OpeningState::setCurrentScene(Scene* scene){

// currentScene = scene; 
//}

//Scene* OpeningState::getCurrentScene(){

//  return currentScene;
//}
