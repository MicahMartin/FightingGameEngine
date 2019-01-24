#ifndef _MenuState_h
#define _MenuState_h

#include "states/StateManager.h"
#include "states/GameState.h"

class MenuState : public GameState{
public:
  MenuState();
  ~MenuState();

  void enter();
  void exit();

  GameState* handleInput(uint16_t inputBits);
  void update(StateManager* stateManager);
  void draw();

private:
  enum menu { VERSUS, CONFIG };
  int menuCounter = 0;
};
#endif
