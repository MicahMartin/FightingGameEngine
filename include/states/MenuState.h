#ifndef _MenuState_h
#define _MenuState_h

#include <vector>
#include "states/GameState.h"
#include "Game.h"

class MenuState : public GameState{
public:
  MenuState(Game* game);
  ~MenuState();

  void enter();
  void exit();

  void pause();
  void resume();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

private:
  Game* game;
};
#endif
