#ifndef _OpeningScreen_h
#define _OpeningScreen_h

#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"
#include "Screen.h"

class OpeningScreen : public Screen{
public:
  OpeningScreen();
  ~OpeningScreen();

  void init();
  void update();
  void draw(SDL_Renderer* renderer);

  void addTexture(GameTexture* gText);
  void removeTexture(int index);
};
#endif
