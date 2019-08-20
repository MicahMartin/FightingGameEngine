#ifndef _CharacterState_h
#define _CharacterState_h

#include "input/VirtualController.h"
class CharacterState {
public:
  virtual void enter() = 0;
  virtual void exit() = 0;
  virtual void handleInput(VirtualController* vc) = 0;
  virtual void update() = 0;
};

#endif
