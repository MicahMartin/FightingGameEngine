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

  // load health bars
  // currentScreen.renderHealthBar(100, 50, 500, 50, p1HpPercent, green, red);
  // currentScreen.renderHealthBar(680, 50, 500, 50, p2HpPercent, green, red);
  // p1HealthBar.damage.loadTexture("../data/images/UI/health_bar/p1_damage_full.png", 50, 20, 500, 200);
  p1HealthBar.bar.loadTexture("../data/images/UI/health_bar/p1_lifebar_empty_cropped.png");
  p1HealthBar.bar.getFileDimensions();
  p1HealthBar.health.loadTexture("../data/images/UI/health_bar/p1_health_full_cropped.png");
  p1HealthBar.health.getFileDimensions();

  p2HealthBar.bar.loadTexture("../data/images/UI/health_bar/p2_lifebar_empty.png", 730, 20, 500, 200);
  p2HealthBar.damage.loadTexture("../data/images/UI/health_bar/p2_damage_full.png", 730, 20, 500, 200);
  p2HealthBar.health.loadTexture("../data/images/UI/health_bar/p2_health_full.png", 730, 20, 500, 200);

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

void FightScreen::renderHealthBar(int x, int y, int w, int h, float percent) {
  // int barWidth = 500;
  // int imgWidth = 2175;
  // int srcStart = percent * imgWidth;

  // int barDisplay = percent * barWidth;
  // int srcStartRemainder = imgWidth - srcStart;
  // int barDisplayRemainder = barWidth - barDisplay;
  // int srcOffset = 432;
  // int xOffset = 150;

  // printf("barDisplay: %d, barDisplayRemainder:%d, srcStartRemainder:%d\n", barDisplay, barDisplayRemainder, srcStartRemainder);
  // SDL_Rect dest = {barDisplayRemainder + srcOffset, 20, barDisplay, 200};
  // SDL_Rect src = {srcStartRemainder + srcOffset, 0, imgWidth, 477};
  //
  // 2014 × 342
  // 671  114
  // 1606 × 167
  // 555 55
  // 330 300
  int barImgWidth = p1HealthBar.bar.imgWidth;
  int barImgHeight = p1HealthBar.bar.imgHeight;
  int barWidth = p1HealthBar.bar.imgWidth/3;
  int barHeight = p1HealthBar.bar.imgHeight/3;

  int healthImgWidth = p1HealthBar.health.imgWidth;
  int healthImgHeight = p1HealthBar.health.imgWidth;
  int healthWidth = healthImgWidth/3;
  int healthHeight = healthImgHeight/3;

  int offsetX = 328/3;
  int offsetY = 130/3;
  int percentOffset = healthImgWidth * percent;
  int widthPercent = healthWidth * percent;
  int remainder = healthWidth - widthPercent;

  SDL_Rect p1BarDest = { 50, 20, barWidth, barHeight };
  SDL_Rect p1HealthDest = { 50+offsetX+remainder, 20+offsetY, widthPercent, healthHeight };
  SDL_Rect p1HealthSrc = { healthImgWidth - (percentOffset), 0, healthImgWidth, healthImgHeight };

  p1HealthBar.bar.render(p1BarDest);
  // p1HealthBar.damage.render();
  p1HealthBar.health.render(p1HealthSrc, p1HealthDest);

  // p2HealthBar.bar.render();
  // p2HealthBar.damage.render();
  // p2HealthBar.health.render();
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
