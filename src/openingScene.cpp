#include "openingScene.h"

OpeningScene::OpeningScene(){
}

OpeningScene::~OpeningScene(){
  
}

void OpeningScene::update(){
  
}

void OpeningScene::init(){
  
}

SDL_Surface* OpeningScene::getCurrentSurface(){
  return currentSurface;
}

void OpeningScene::setSurface(SDL_Surface* surface) {
  currentSurface = surface;
}
