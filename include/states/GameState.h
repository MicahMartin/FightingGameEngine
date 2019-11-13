#ifndef _GameState_h
#define _GameState_h

#include <iostream>
#include "input/InputManager.h"
#include "StateManager.h"

class GameState{
public:
  virtual ~GameState() { };

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void resume() = 0;
  virtual void pause() = 0;

  virtual void handleInput() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;
  long gameTime = 0;
  const char* stateName = "";
protected:
  bool paused = false;
  StateManager* stateManager = StateManager::getInstance();
  InputManager* inputManager = InputManager::getInstance();
};
#endif
