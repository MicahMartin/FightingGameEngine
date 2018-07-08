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
    std::cout << "Heres the current byte of input" << std::bitset<32>(inputManager->getInputByte()) << std::endl;
}

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
