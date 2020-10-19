#ifndef _NetConnectState_h
#define _NetConnectState_h

#include "states/GameState.h"
#include "util/Menu.h"
#include "screens/NetConnectScreen.h"

class NetConnectState : public GameState {
public:
  NetConnectState();
  ~NetConnectState();

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
  NetConnectScreen connectScreen;
};
#endif
