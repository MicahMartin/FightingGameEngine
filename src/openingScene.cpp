#include "openingScene.h"
#include <SDL2_image/SDL_image.h>

OpeningScene::OpeningScene(){
}

OpeningScene::~OpeningScene(){
}

void OpeningScene::update(){
  
}

void OpeningScene::init(){
 // set the screen to all white 
 currentSurface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
 SDL_FillRect(currentSurface, NULL, SDL_MapRGBA(currentSurface->format, 115, 15, 118, 1));
}

SDL_Surface* OpeningScene::getCurrentSurface(){
  return currentSurface;
}

void OpeningScene::setSurface(SDL_Surface* surface) {
  currentSurface = surface;
}
