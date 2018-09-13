#ifndef _MenuState_h
#define _MenuState_h

#include <vector>
#include "states/GameState.h"
#include "Game.h"

class MenuState : public GameState{
public:
  MenuState(StateCollection* stateCollection);
  ~MenuState();

  void enter();
  void exit();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

private:
  StateCollection* stateCollection;
};
#endif
