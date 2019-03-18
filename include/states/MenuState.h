#ifndef _MenuState_h
#define _MenuState_h

#include <functional>
#include "states/GameState.h"
#include "states/FightState.h"
#include "states/ConfigState.h"
#include "screens/MenuScreen.h"

class MenuState : public GameState {
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

  struct MenuCursor {
    int position = 0;
    GameTexture cursorTexture;
  };

  struct Menu {
    Menu(){
      menuCursor.cursorTexture.loadTexture("../../data/images/cursor.png");
      menuCursor.cursorTexture.setDimensions(150,100,25,25);

    }

    std::vector<MenuItem> menuItemArray;
    MenuCursor menuCursor;

    void moveCursorDown(){
      printf("moving cursor down%d\n", menuCursor.position);
      (menuCursor.position == menuItemArray.size() - 1) ? menuCursor.position = 0 : menuCursor.position++;
    };

    void moveCursorUp(){
      printf("moving cursor up%d\n", menuCursor.position);
      menuCursor.position == 0 ? menuCursor.position = (menuItemArray.size() - 1) : menuCursor.position--;
    };

    void activate(){
      menuItemArray.at(menuCursor.position).doSomething();
    };
  };
  Menu mainMenu;
  FightState fightState;
  ConfigState configState;
};
#endif
