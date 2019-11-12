#include "screens/FightScreen.h"

FightScreen::FightScreen(){
  printf("the address of the screen %p\n", this);

  int windowWidth = graphics->getWindowWidth();
  stage.loadTexture("../data/images/purple_stage.png");
  stage.setDimensions(0, 0, windowWidth*3, graphics->getWindowHeight());

  for (int i = 0; i < 9; i++) {
    numbers[i].loadTexture("../data/images/font/num.png");
  }

  addTexture(&stage);
}
FightScreen::~FightScreen(){}

void FightScreen::init(){}

void FightScreen::update(){}

void FightScreen::draw(){

  for (auto gameTexture : textureList) {
    gameTexture->render(true);
  }
}

void FightScreen::addTexture(GameTexture* gText){
  Screen::addTexture(gText);
}

void FightScreen::removeTexture(int index){
  Screen::removeTexture(index);
}

void FightScreen::renderHealthBar(int x, int y, int w, int h, float percent, SDL_Color fgColor, SDL_Color bgColor) {
  // TODO: Call this once
  SDL_Color old;
  SDL_GetRenderDrawColor(graphics->getRenderer(), &old.r, &old.g, &old.g, &old.a);
  SDL_Rect bgrect = { x, y, w, h };
  SDL_SetRenderDrawColor(graphics->getRenderer(), bgColor.r, bgColor.g, bgColor.b, bgColor.a);
  SDL_RenderFillRect(graphics->getRenderer(), &bgrect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), fgColor.r, fgColor.g, fgColor.b, fgColor.a);
  int pw = (int)((float)w * percent);
  int px = x + (w - pw);
  SDL_Rect fgrect = { px, y, pw, h };
  SDL_RenderFillRect(graphics->getRenderer(), &fgrect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), old.r, old.g, old.b, old.a);
}

void FightScreen::renderComboCount(bool side, int count) {
}
