#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "screens/OpeningScreen.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();

  void enter();
  void exit();

  void pause();
  void resume();

  void handleInput();
  void update();
  void draw();

private:
  OpeningScreen openingScreen;
};
#endif
