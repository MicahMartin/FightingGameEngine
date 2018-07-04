#include "openingState.h"
#include "openingScene.h"
#include "input.h"
#include <bitset>
#include <iostream>

OpeningState::OpeningState(){

  printf("entering intro state \n");
  currentScene = new OpeningScene();
}

OpeningState::~OpeningState(){
  
}
void OpeningState::update(){
  //printf("Updating intro state \n");
  printf("updating with no input!\n");

  currentScene->update();
}

void OpeningState::update(InputManager* inputManager){
  //printf("Updating intro state \n");
    printf("Heres the current byte of input %d\n", inputManager->getInputByte());
}

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
