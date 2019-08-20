#ifndef _GameState_h
#define _GameState_h

#include <iostream>

class GameState {
public:
  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void resume() = 0;
  virtual void pause() = 0;

  virtual void handleInput() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

protected:
  bool paused = false;
};
#endif
