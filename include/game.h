#ifndef _Game_h
#define _Game_h

#include "graphics/Graphics.h"
#include "input/InputManager.h"
#include "states/StateManager.h"
#include "observer/Observer.h"

class Game : public Observer{
public:
  Game();
  ~Game();

  void init();
  void update();

  bool stillRunning();

  Graphics* getGraphics();
  InputManager* getInputManager();
  StateManager* getStateManager();

  // observer
  void onNotify(const char* message);

private:
  bool running;
  int gameTime = 0;
  Graphics coreGraphics;
  InputManager inputManager;
  StateManager stateManager;
};
#endif
