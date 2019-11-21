#ifndef _MenuScreen_h
#define _MenuScreen_h

#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"
#include "Screen.h"

class MenuScreen : public Screen{
public:
  MenuScreen();
  ~MenuScreen();

  void init();
  void update();
  void draw();

private:
  GameTexture menuBackground;
  int screenTime;
};
#endif
