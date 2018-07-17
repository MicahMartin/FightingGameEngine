#include "openingScene.h"
#include "gameTexture.h"
#include <SDL2_image/SDL_image.h>

OpeningScene::OpeningScene(){
  printf("Entering opening scene \n");
  // set the screen to all white 
  GameTexture* background = new GameTexture();
  GameTexture* sprite = new GameTexture();
  
}

OpeningScene::~OpeningScene(){
}

void OpeningScene::update(){

 // Fill screen with purple 
 //printf("updating the scene \n");
 // foreach in texture vector, call render
}

void OpeningScene::init(){}


void OpeningScene::addTexture(GameTexture* gText) {
  textureList.push_back(gText);
}
void OpeningScene::removeTexture(int index) {
  textureList.erase(textureList.begin() + index);
}
