#ifndef _GameState_h
#define _GameState_h

#include <iostream>
#include <vector>
#include "input/VirtualController.h"

class StateManager;

class GameState{
public:
  virtual ~GameState() { };

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void resume() = 0;
  virtual void pause() = 0;

  virtual void handleInput(StateManager* sm, VirtualController* vc) = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

protected:
  bool paused = false;
};
#endif
