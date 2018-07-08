#include "openingScene.h"
#include <SDL2_image/SDL_image.h>

OpeningScene::OpeningScene(){
  printf("Entering opening scene \n");
  // set the screen to all white 
}

OpeningScene::~OpeningScene(){
}

void OpeningScene::update(){

 // Fill screen with purple 
 //printf("updating the scene \n");
}

void OpeningScene::init(){}

SDL_Texture* OpeningScene::getCurrentTexture(){
  return sceneTexture;
}

void OpeningScene::setTexture(SDL_Texture* texture) {
  sceneTexture = texture;
}
