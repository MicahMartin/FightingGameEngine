#ifndef _OpeningScreen_h
#define _OpeningScreen_h

#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"
#include "Screen.h"

class OpeningScreen : public Screen {
public:
  OpeningScreen();
  ~OpeningScreen();

  void init();
  void update();
  void draw();

private:
  GameTexture background;
  GameTexture title;
  int screenTime;
};
#endif
