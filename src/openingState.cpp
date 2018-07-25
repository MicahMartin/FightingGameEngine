#include "openingState.h"
#include "openingScene.h"
#include "game.h"
#include <bitset>
#include <iostream>

OpeningState::OpeningState(){ }

OpeningState::~OpeningState(){
}

void OpeningState::update(Game* stateManager){
  int stickState = virtualController->getState();
  // printf("inside the opening state, heres current state of virtual controller %d\n", stickState);
}

void OpeningState::enter(Game* stateManager) {
  printf("entering intro state \n");
  virtualController = stateManager->getInputManager()->getVirtualController();
  setCurrentScene(new OpeningScene);
}

void OpeningState::exit() { 

  printf("leaving the opening state! \n");
  // cleanup
}

void OpeningState::pause() { }

void OpeningState::resume() { }

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
