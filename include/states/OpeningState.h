#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "states/MenuState.h"
#include "StateManager.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();

  void enter();
  void exit();

  void handleInput(StateManager* sm, VirtualController* vc);
  void update();
  void draw();

private:
  MenuState mainMenu;
};
#endif
