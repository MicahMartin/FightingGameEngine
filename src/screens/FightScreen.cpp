#include "screens/FightScreen.h"
#include <sstream>

FightScreen::FightScreen(){
  printf("the address of the screen %p\n", this);

  int windowWidth = graphics->getWindowWidth();
  p1ComboCountPosition = { 150, graphics->getWindowHeight()/3, 100, 150 };
  p2ComboCountPosition = { graphics->getWindowWidth() - 150, graphics->getWindowHeight()/3, 100, 150 };
  p1ComboCountPositionSecond = { 250, graphics->getWindowHeight()/3, 100, 150 };
  p2ComboCountPositionSecond = { graphics->getWindowWidth() - 250, graphics->getWindowHeight()/3, 100, 150 };
  stage.loadTexture("../data/images/purple_stage.png");
  stage.setDimensions(0, 0, windowWidth*3, graphics->getWindowHeight());

  for (int i = 0; i <= 9; i++) {
    char n = i + '0';
    std::stringstream numPath;

    numPath << "../data/images/font/" << n << ".png";
    if (!numbers[i].loadTexture(numPath.str().c_str())) {
      printf("error loading fucking font %d\n", i);
    }
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

void FightScreen::renderComboCount(bool p1Side, int count) {
  printf("wtff\n");
  int tens = (count/10) % 10;
  int ones = (count/1) % 10;
  printf("wtff %d %d\n", tens, ones);
  if (p1Side) {
    if (count > 9 && count < 100) {
      printf("count grater than 9\n");
      numbers[tens].render(p1ComboCountPosition);
      numbers[ones].render(p1ComboCountPositionSecond);
    } else {
      numbers[count].render(p1ComboCountPosition);
    }
  } else {
    if (count > 9 && count < 100) {
      numbers[tens].render(p2ComboCountPosition);
      numbers[ones].render(p2ComboCountPositionSecond);
    } else {
      numbers[count].render(p2ComboCountPosition);
    }
  }
}

void FightScreen::renderInputHistory(bool side) {
}
