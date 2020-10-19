#ifndef _Menu_h
#define _Menu_h

#include <iostream>
#include <functional>
#include <vector>
#include <SDL_mixer.h>
#include "input/VirtualController.h"
#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"


struct MenuItem {
  MenuItem(const char* title, const char* message, int textWidth, int textHeight, std::function<void()> callBack, std::function<void()> update)
    : callBack(callBack), update(update), title(title) {
    itemTexture.setText(message);
    itemTexture.setDimensions(0, 0, textWidth, textHeight);
  };

  void draw(){
    itemTexture.render();
  };

  void onActivate(){
    printf("%s bouta do some stuff \n", title);
    callBack();
  };

  const char* title;
  GameTexture itemTexture;
  std::function<void()> callBack;
  std::function<void()> update;
};

struct MenuCursor {
  int position = 0;
  GameTexture cursorTexture;
};

struct DisplayElem {
  DisplayElem(){};
  DisplayElem(const char* title, const char* texturePath, int textWidth, int textHeight, int positionX, int positionY, bool display) 
    : title(title), display(display) {
    elemTexture.loadTexture(texturePath);
    elemTexture.setDimensions(positionX, positionY, textWidth, textHeight);
  };
  void draw(){
    elemTexture.render();
  };

  const char* title;
  bool display = false;
  GameTexture elemTexture;
};

class Menu {
public:
  Menu();
  Menu(int width, int height, SDL_Color borderColor, SDL_Color bodyColor, const char* cursorTexturePath);
  ~Menu();

  void handleInput();
  void draw();

  void addMenuItem(const char* title, const char* texturePath, int textWidth, int textHeight, std::function<void()> callBack, std::function<void()> update);
  void activateMenuItem();

  void moveCursorUp();
  void moveCursorDown();

  void addDisplayElem(const char* title, const char* texturePath, int textWidth, int textHeight, int positionX, int positionY, bool display);
  DisplayElem* getDisplayElem(const char* title);

  VirtualController* vc;
  Mix_Chunk* menuMove = NULL;
  Mix_Chunk* menuSelect = NULL;
  std::unordered_map<std::string, DisplayElem> displayElems;
private:
  int menuWidth;
  int menuHeight;
  int menuXPos;
  int menuYPos;

  MenuCursor menuCursor;
  std::vector<MenuItem> menuItemArray;
  SDL_Color borderColor;
  SDL_Color bodyColor;
  SDL_Rect menuRect;
  SDL_Rect borderRect;
  SDL_Renderer* renderer = Graphics::getInstance()->getRenderer();
};
#endif
