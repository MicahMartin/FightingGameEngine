#include "screens/MenuScreen.h"
#include "graphics/GameTexture.h"
#include <SDL_image.h>

MenuScreen::MenuScreen(){
  printf("menu screen constructor\n");

  menuBackground.loadTexture("../data/images/menuBackground.png");
  menuBackground.setDimensions(0,0, graphics->getWindowWidth(), graphics->getWindowHeight());

  addTexture(&menuBackground);
  screenTime = 0;
}

MenuScreen::~MenuScreen(){
}

void MenuScreen::init(){ 
}

void MenuScreen::update(){
  screenTime++;
}

void MenuScreen::draw() {
  for (auto gameTexture : textureList) {
    gameTexture->render();
  }
}
