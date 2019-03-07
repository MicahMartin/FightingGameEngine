#ifndef _MenuState_h
#define _MenuState_h

#include "states/GameState.h"
#include "input/VirtualController.h"

class MenuState : public GameState{
public:
  MenuState();
  ~MenuState();

  void enter();
  void exit();

  GameState* handleInput(VirtualController* vc);
  void update();
  void draw();

private:
  enum menu { VERSUS, CONFIG };
  int menuCounter = 0;
};
#endif
