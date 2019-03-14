#include "game_objects/Stage.h"

Stage::Stage(Screen* currentScreen){
  background = new GameTexture();
  background->loadTexture("../data/images/newGrid.png");
  background->setDimensions(0,0,640,480);

  currentScreen->addTexture(background);
  printf("stage setup complete, address of stage %p \n", currentScreen);
};

Stage::~Stage(){};

