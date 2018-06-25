#include "openingState.h"
#include "openingScene.h"

OpeningState::OpeningState(){

  printf("entering intro state \n");
  currentScene = new OpeningScene();
}

OpeningState::~OpeningState(){
  
}

void OpeningState::update(){
  printf("Updating intro state \n");

  currentScene->update();
}

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
