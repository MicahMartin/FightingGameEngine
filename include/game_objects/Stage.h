#ifndef _Stage_h
#define _Stage_h

#include "graphics/GameTexture.h"
#include "screens/Screen.h"

class Stage {

public:
  Stage(Screen* currentScreen);
  ~Stage();
  void init();

private:
  GameTexture* background;
};

#endif
