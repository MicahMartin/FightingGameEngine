#include "openingState.h"
#include "openingScene.h"

OpeningState::OpeningState(){

  currentScene = new OpeningScene();
  currentScene->init();
}

OpeningState::~OpeningState(){
  
}

void OpeningState::update(){

  currentScene->update();
}

void OpeningState::setCurrentScene(Scene* scene){

 currentScene = scene; 
}

Scene* OpeningState::getCurrentScene(){

  return currentScene;
}
