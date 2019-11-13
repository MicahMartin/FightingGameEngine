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

  void addTexture(GameTexture* gText);
  void removeTexture(int index);
private:
  GameTexture menuBackground;
  GameTexture versus;
  GameTexture config;
  int screenTime;
};
#endif
