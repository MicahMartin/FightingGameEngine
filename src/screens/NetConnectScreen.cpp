#include "screens/NetConnectScreen.h"
#include "graphics/GameTexture.h"
#include <SDL_image.h>

NetConnectScreen::NetConnectScreen(){
  printf("menu screen constructor\n");

  menuBackground.loadTexture("../data/images/menuBackground.png");
  menuBackground.setDimensions(0,0, graphics->getWindowWidth(), graphics->getWindowHeight());

  addTexture(&menuBackground);
  screenTime = 0;
}

NetConnectScreen::~NetConnectScreen(){
}

void NetConnectScreen::init(){ 
}

void NetConnectScreen::update(){
  screenTime++;
}

void NetConnectScreen::draw() {
  for (auto gameTexture : textureList) {
    gameTexture->render();
  }
}
