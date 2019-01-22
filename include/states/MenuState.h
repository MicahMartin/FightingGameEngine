#ifndef _MenuState_h
#define _MenuState_h

#include "states/StateManager.h"
#include "states/GameState.h"

class MenuState : public GameState{
public:
  MenuState(StateManager* stateManager);
  ~MenuState();

  void enter();
  void exit();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

private:
  StateManager* stateManager;
};
#endif
