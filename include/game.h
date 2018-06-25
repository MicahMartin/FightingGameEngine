#ifndef _game_h
#define _game_h

#include "graphics.h"
#include "inputManager.h"

class Game{
public:
  Game();
  ~Game();
  void init();
  void run();

private:
  /* data */
  Graphics coreGraphics;
  InputManager inputManager;
};
#endif /* ifndef _game_h */
