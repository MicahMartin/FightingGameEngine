#ifndef _MenuState_h
#define _MenuState_h

#include <functional>
#include "states/GameState.h"
#include "screens/MenuScreen.h"

class MenuState : public GameState{
public:
  MenuState();
  ~MenuState();

  void enter();
  void exit();

  void pause();
  void resume();

  void handleInput(VirtualController* vc);
  void update();
  void draw();

private:
  MenuScreen menuScreen;

  struct MenuItem {
    const char* title;
    void doSomething(){
      printf("%s bouta do some stuff \n", title);
      callBack();
    };
    std::function<void()> callBack;
  };

  struct Menu {
    std::vector<MenuItem> menuItemArray;
    void moveCursorDown(){
      printf("moving cursor down%d\n", menuCounter);
      (menuCounter == menuItemArray.size() - 1) ? menuCounter = 0 : menuCounter++;
    };

    void moveCursorUp(){
      printf("moving cursor up%d\n", menuCounter);
      menuCounter == 0 ? menuCounter = (menuItemArray.size() - 1) : menuCounter--;
    };

    void activate(){
      menuItemArray.at(menuCounter).doSomething();
    };

    private:
    int menuCounter = 0;
  };
  Menu mainMenu;
  int foobar = 10;
};
#endif
