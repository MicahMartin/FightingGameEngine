#include "screens/OpeningScreen.h"
#include "graphics/GameTexture.h"
#include <SDL2_image/SDL_image.h>

OpeningScreen::OpeningScreen(Graphics* graphics) : graphics(graphics){
  printf("Entering opening screen \n");

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

OpeningScreen::~OpeningScreen(){
}

void OpeningScreen::init(){ }

void OpeningScreen::update(){
 // Fill screen with purple 
 //printf("updating the screen \n");
 // foreach in texture vector, call render
}

void OpeningScreen::draw() {
  for (auto gameTexture : textureList) {
    gameTexture->render(renderer);
  }
}



void OpeningScreen::addTexture(GameTexture* gText) {
  textureList.push_back(gText);
}
void OpeningScreen::removeTexture(int index) {
  textureList.erase(textureList.begin() + index);
}
