#include "screens/FightScreen.h"
#include <sstream>

FightScreen::FightScreen(){
  printf("the address of the screen %p\n", this);

  int windowWidth = graphics->getWindowWidth();
  p2DirectionDrawXPos = graphics->getWindowWidth() - 50;
  p1ComboCountPosition = { 150, graphics->getWindowHeight()/3, 80, 160 };
  p2ComboCountPosition = { graphics->getWindowWidth() - 150, graphics->getWindowHeight()/3, 80, 160 };
  p1ComboCountPositionSecond = { 250, graphics->getWindowHeight()/3, 80, 160 };
  p2ComboCountPositionSecond = { graphics->getWindowWidth() - 250, graphics->getWindowHeight()/3, 80, 160 };

  stage.loadTexture("../data/images/purple_stage.png");
  stage.setDimensions(0, 0, windowWidth*3, graphics->getWindowHeight());
  sky.loadTexture("../data/images/sky.png");
  sky.setDimensions(0, -graphics->getWindowHeight(), windowWidth*3, graphics->getWindowHeight()*2);
  addTexture(&sky);
  addTexture(&stage);

  directions[0].loadTexture("../data/images/font/right.png");
  directions[1].loadTexture("../data/images/font/left.png");
  directions[2].loadTexture("../data/images/font/up.png");
  directions[3].loadTexture("../data/images/font/down.png");
  buttons[0].loadTexture("../data/images/font/punch.png");
  buttons[1].loadTexture("../data/images/font/kick.png");
  buttons[2].loadTexture("../data/images/font/slash.png");
  buttons[3].loadTexture("../data/images/font/dust.png");
  for (int i = 0; i <= 9; i++) {
    char n = i + '0';
    std::stringstream numPath;

    numPath << "../data/images/font/" << n << ".png";
    if (!numbers[i].loadTexture(numPath.str().c_str())) {
      printf("error loading fucking font %d\n", i);
    }
  }
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
  int tens = (count/10) % 10;
  int ones = (count/1) % 10;
  if (p1Side) {
    if (count > 9 && count < 100) {
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

void FightScreen::renderInputHistory(bool p1Side, boost::circular_buffer<InputEvent>& events) {
  for (int i = 0; i < events.size(); ++i) {
    SDL_Rect drawPosition = inputHistoryPositions[i];
    if (!p1Side) {
      drawPosition.x = p2DirectionDrawXPos;
    }
    switch ((&events[i])->inputBit) {
      case RIGHT:
        directions[0].render(drawPosition);
        break;
      case LEFT:
        directions[1].render(drawPosition);
        break;
      case UP:
        directions[2].render(drawPosition);
        break;
      case DOWN:
        directions[3].render(drawPosition);
        break;
      case UPRIGHT:
        directions[2].render(drawPosition, 45);
        break;
      case UPLEFT:
        directions[2].render(drawPosition, -45);
        break;
      case DOWNRIGHT:
        directions[3].render(drawPosition, -45);
        break;
      case DOWNLEFT:
        directions[3].render(drawPosition, 45);
        break;
      case LP:
        buttons[0].render(drawPosition);
        break;
      case LK:
        buttons[1].render(drawPosition);
        break;
      case MP:
        buttons[2].render(drawPosition);
        break;
      case MK:
        buttons[3].render(drawPosition);
        break;
      default:
        break;
    }
  }
}
