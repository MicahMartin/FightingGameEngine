#include "scenes/OpeningScene.h"
#include "graphics/GameTexture.h"
#include <SDL2_image/SDL_image.h>

OpeningScene::OpeningScene(Graphics* graphics) : graphics(graphics){
  printf("Entering opening scene \n");

  renderer = graphics->getRenderer();
  // set the screen to all white 
  GameTexture* background = new GameTexture();
  background->loadTexture("../data/background.jpg", renderer);
  background->setDimensions(0,0,400,400);

  GameTexture* sprite = new GameTexture();
  sprite->loadTexture("../data/megaMan.jpg", renderer);
  sprite->setDimensions(0,0,50,50);

  addTexture(background);
  addTexture(sprite);
}

OpeningScene::~OpeningScene(){
}

void OpeningScene::init(){ }

void OpeningScene::update(){
 // Fill screen with purple 
 //printf("updating the scene \n");
 // foreach in texture vector, call render
}

void OpeningScene::draw() {
  for (auto gameTexture : textureList) {
    gameTexture->render(renderer);
  }
}



void OpeningScene::addTexture(GameTexture* gText) {
  textureList.push_back(gText);
}
void OpeningScene::removeTexture(int index) {
  textureList.erase(textureList.begin() + index);
}
