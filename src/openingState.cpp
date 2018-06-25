#include "openingState.h"
#include "openingScene.h"

OpeningState::OpeningState(){
  currentScene = new OpeningScene();
}

OpeningState::~OpeningState(){
  
}

void OpeningState::update() {

  currentScene->update();
  
}
