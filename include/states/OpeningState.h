#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "input/VirtualController.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();

  void enter();
  void exit();

  GameState* handleInput(VirtualController* vc);
  void update();
  void draw();

private:
};
#endif
