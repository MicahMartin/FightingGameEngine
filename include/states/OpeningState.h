#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/StateManager.h"
#include "states/GameState.h"

class OpeningState : public GameState{
public:
  OpeningState(StateManager* stateManager);
  ~OpeningState();

  void enter();
  void exit();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

private:
  StateManager* stateManager;
};
#endif
