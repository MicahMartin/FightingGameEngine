#include "openingState.h"
#include "openingScene.h"
#include "game.h"
#include <bitset>
#include <iostream>

OpeningState::OpeningState(){ }

OpeningState::~OpeningState(){
}

void OpeningState::update(Game* stateManager){
}

void OpeningState::enter(Game* stateManager) {
  printf("entering intro state \n");
  setCurrentScene(new OpeningScene);
}

void OpeningState::exit() { }

void OpeningState::pause() { }

void OpeningState::resume() { }

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
