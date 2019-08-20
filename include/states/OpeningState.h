#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "states/MenuState.h"
#include "screens/OpeningScreen.h"

class OpeningState : public GameState {
public:
  OpeningState();
  ~OpeningState();

  void enter();
  void exit();

  void pause();
  void resume();

  void handleInput(VirtualController* input);
  void update();
  void draw();

private:
  MenuState* mainMenu;
  OpeningScreen* openingScreen;
};
#endif
