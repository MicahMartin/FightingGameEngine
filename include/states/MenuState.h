#ifndef _MenuState_h
#define _MenuState_h

#include "states/GameState.h"
#include "screens/MenuScreen.h"
#include "StateManager.h"

class MenuState : public GameState{
public:
  MenuState();
  ~MenuState();

  void enter();
  void exit();

  void pause();
  void resume();

  void handleInput(StateManager* sm, VirtualController* vc);
  void update();
  void draw();

private:
  MenuScreen menuScreen;
  struct MenuItem {
    void doSomething(){
      printf("%s doing shit\n", title);
    };
    const char* title;
  };
  struct Menu {
    std::vector<MenuItem> menuItemArray;
    void moveCursorDown(){
      printf("moving cursor down%d\n", menuCounter);
      if(menuCounter == menuItemArray.size() - 1){
        menuCounter = 0;
      } else {
        menuCounter++;
      }
    };

    void moveCursorUp(){
      printf("moving cursor up%d\n", menuCounter);
      if(menuCounter == 0){
        menuCounter = menuItemArray.size()-1;
      } else {
        menuCounter--;
      }
    };

    void activate(){
      menuItemArray.at(menuCounter).doSomething();
    };

    private:
    int menuCounter = 0;
  };
  Menu mainMenu;
};
#endif
