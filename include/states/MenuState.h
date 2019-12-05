#ifndef _MenuState_h
#define _MenuState_h

#include "states/GameState.h"
#include "util/Menu.h"
#include "screens/MenuScreen.h"


class MenuState : public GameState {
public:
  MenuState();
  ~MenuState();

  void enter();
  void exit();

  void pause();
  void resume();

  void handleInput();
  void update();
  void draw();

private:
  Menu* activeMenu;
  Menu mainMenu;
  Menu configMenu;
  MenuScreen menuScreen;
};
#endif
