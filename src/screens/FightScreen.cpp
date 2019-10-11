#include "screens/FightScreen.h"

FightScreen::FightScreen(){
  printf("the address of the screen %p\n", this);

  GameTexture* stage = new GameTexture();
  stage->loadTexture("../data/images/purple_stage.png");
  stage->setDimensions(0,0, graphics->getWindowWidth()*3, graphics->getWindowHeight());

  addTexture(stage);
};
FightScreen::~FightScreen(){};

void FightScreen::init(){};
void FightScreen::update(){};

void FightScreen::draw(){
  for (auto gameTexture : textureList) {
    gameTexture->render();
  }
};

void FightScreen::addTexture(GameTexture* gText){
  Screen::addTexture(gText);
};
void FightScreen::removeTexture(int index){
  Screen::removeTexture(index);
};
