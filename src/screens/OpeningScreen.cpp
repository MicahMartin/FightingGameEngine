#include "screens/OpeningScreen.h"
#include "graphics/GameTexture.h"
#include <SDL2_image/SDL_image.h>

OpeningScreen::OpeningScreen(){
}

OpeningScreen::~OpeningScreen(){
}

void OpeningScreen::init(){ 
  printf("init opening screen \n");

  GameTexture* background = new GameTexture();
  background->loadTexture("../data/images/background.jpg");
  background->setBlendMode(SDL_BLENDMODE_BLEND);
  background->setTransperancy(0);
  background->setDimensions(0,0,640,480);

  GameTexture* title = new GameTexture();
  title->loadTexture("../data/images/title.png");
  title->setBlendMode(SDL_BLENDMODE_BLEND);
  title->setTransperancy(0);
  title->setDimensions(150,100,340,100);

  addTexture(background);
  addTexture(title);
  screenTime = 0;
}

void OpeningScreen::update(){
  screenTime++;
}

void OpeningScreen::draw() {
  for (auto gameTexture : textureList) {
    if(gameTexture->getTransperancy() != 255){
      gameTexture->incTransperancy();
    }
    gameTexture->render();
  }
}



void OpeningScreen::addTexture(GameTexture* gText) {
  Screen::addTexture(gText);
}
void OpeningScreen::removeTexture(int index) {
  Screen::removeTexture(index);
}
