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

void OpeningState::update(Input lastInput){
  //printf("Updating intro state \n");
  std::cout << "Heres the current inputs bit" << std::bitset<32>(lastInput.getKeyCode()) << std::endl;
  currentScene->update();
}

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
