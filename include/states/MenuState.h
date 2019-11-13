#ifndef _MenuState_h
#define _MenuState_h

#include <functional>
#include "states/GameState.h"
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
  struct MenuItem {
    MenuItem(const char* title, std::function<void()> callBack): callBack(callBack), title(title){};

    void onActivate(){
      printf("%s bouta do some stuff \n", title);
      callBack();
    };

    std::function<void()> callBack;
    const char* title;
  };

  struct MenuCursor {
    void moveCursorGraphic(int y) {
      std::pair<int, int> coords = cursorTexture.getCords();
      cursorTexture.setCords(coords.first, coords.second + y);
    }

    int position = 0;
    GameTexture cursorTexture;
  };

  struct Menu {
    Menu(){
      menuCursor.cursorTexture.loadTexture("../data/images/cursor.png");
      menuCursor.cursorTexture.setDimensions(150,100,25,25);
    }

    void moveCursorDown() {
      printf("moving cursor down%d\n", menuCursor.position);
      if(menuCursor.position == menuItemArray.size() - 1) {
        menuCursor.position = 0;
        menuCursor.moveCursorGraphic(-150);
      } else {
        menuCursor.position++;
        menuCursor.moveCursorGraphic(150);
      }
    };

    void moveCursorUp() {
      printf("moving cursor up%d\n", menuCursor.position);
      if(menuCursor.position == 0) {
        menuCursor.position = menuItemArray.size() - 1;
        menuCursor.moveCursorGraphic(150);
      } else {
        menuCursor.position--;
        menuCursor.moveCursorGraphic(-150);
      }
    };

    void activate() {
      menuItemArray.at(menuCursor.position).onActivate();
    };

    std::vector<MenuItem> menuItemArray;
    MenuCursor menuCursor;
  };

  Menu mainMenu;
  MenuScreen menuScreen;
};
#endif
