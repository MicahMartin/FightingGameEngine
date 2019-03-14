#ifndef _ConfigState_h
#define _ConfigState_h

#include "states/GameState.h"

class ConfigState : public GameState{
public:
  ConfigState();
  ~ConfigState();

  void enter();
  void exit();

  void pause();
  void resume();

  void handleInput(VirtualController* vc);
  void update();
  void draw();

private:
};
#endif
