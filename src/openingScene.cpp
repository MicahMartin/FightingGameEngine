#include "openingScene.h"
#include <SDL2_image/SDL_image.h>

OpeningScene::OpeningScene(){
  printf("Entering opening scene \n");
  // set the screen to all white 
  currentSurface = SDL_CreateRGBSurface(0, 640, 480, 32, 0, 0, 0, 0);
}

OpeningScene::~OpeningScene(){
}

void OpeningScene::update(){

 // Fill screen with purple 
 SDL_FillRect(currentSurface, NULL, SDL_MapRGBA(currentSurface->format, 115, 15, 118, 1));
 //printf("updating the scene \n");
}

void OpeningScene::init(){}

SDL_Surface* OpeningScene::getCurrentSurface(){
  return currentSurface;
}

void OpeningScene::setSurface(SDL_Surface* surface) {
  currentSurface = surface;
}
