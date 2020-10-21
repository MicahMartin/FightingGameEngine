#ifndef _Game_h
#define _Game_h

#include "graphics/Graphics.h"
#include "input/InputManager.h"
#include "input/VirtualController.h"
#include "states/StateManager.h"
#include "observer/Observer.h"
#include "states/FightState.h"

class Game : public Observer {
public:
  Game();
  ~Game();

  void update();
  void draw();

  // observer
  void onNotify(const char* message);

  double inputLength;
  double handleInputLength;
  double updateLength;
  double drawLength;
  double clearLength;
  double stateDrawLength;
  int gameTime = 0;
  bool running = true;
  bool inFightState = false;
  StateManager* stateManager = StateManager::getInstance();
private:
  Graphics* graphics = Graphics::getInstance();
  InputManager* inputManager = InputManager::getInstance();
  VirtualController virtualControllers[2];
};
#endif
