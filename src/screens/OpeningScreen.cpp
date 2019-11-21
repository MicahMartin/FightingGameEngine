#include "screens/OpeningScreen.h"
#include "graphics/GameTexture.h"
#include <SDL2_image/SDL_image.h>

OpeningScreen::OpeningScreen() {
  printf("init opening screen \n");

  background.loadTexture("../data/images/background.jpg");
  background.setBlendMode(SDL_BLENDMODE_BLEND);
  background.setTransperancy(0);
  background.setDimensions(0,0, graphics->getWindowWidth(), graphics->getWindowHeight());

  title.loadTexture("../data/images/title.png");
  title.setBlendMode(SDL_BLENDMODE_BLEND);
  title.setTransperancy(0);
  title.setDimensions(150,100,340,100);

  addTexture(&background);
  addTexture(&title);
  screenTime = 0;
}

OpeningScreen::~OpeningScreen() {
}

void OpeningScreen::init(){ 
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
