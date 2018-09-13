#ifndef _OpeningState_h
#define _OpeningState_h

#include <vector>
#include "states/GameState.h"
#include "states/StateCollection.h"
#include "screens/OpeningScreen.h"

class OpeningState : public GameState{
public:
  OpeningState(StateCollection* stateCollection);
  ~OpeningState();

  void enter();
  void exit();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

private:
  StateCollection* stateCollection;
};
#endif
