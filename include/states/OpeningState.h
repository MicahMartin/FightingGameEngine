#ifndef _OpeningState_h
#define _OpeningState_h

#include <vector>
#include "states/GameState.h"
#include "screens/OpeningScreen.h"
#include "Game.h"

class OpeningState : public GameState{
public:
  OpeningState(Game* game);
  ~OpeningState();

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
