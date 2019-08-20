#ifndef _Game_h
#define _Game_h

#include "graphics/Graphics.h"
#include "input/InputManager.h"
#include "input/VirtualController.h"
#include "states/StateManager.h"
#include "observer/Observer.h"

class Game : public Observer {
public:
  Game();
  ~Game();

  void init();
  void update();

  bool stillRunning();
  // observer
  void onNotify(const char* message);

private:
  bool running = true;
  int gameTime = 0;
  Graphics& graphics = Graphics::getInstance();
  StateManager* stateManager = StateManager::getInstance();
  InputManager* inputManager;
  VirtualController virtualControllers[3];
};
#endif
