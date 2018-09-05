#ifndef _OpeningScreen_h
#define _OpeningScreen_h

#include <vector>
#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"
#include "Screen.h"

class OpeningScreen : public Screen{
public:
  OpeningScreen(Graphics* graphics);
  ~OpeningScreen();

  void init();
  void update();
  void draw();

  void addTexture(GameTexture* gText);
  void removeTexture(int index);

private:
  std::vector<GameTexture*> textureList;
  Graphics* graphics;
  SDL_Renderer* renderer;
};
#endif
