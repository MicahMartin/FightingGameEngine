#ifndef _GameState_h
#define _GameState_h

#include <iostream>
#include <vector>
#include "screens/Screen.h"

class GameState{
public:
  virtual ~GameState() { };

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  void pushState(GameState* state) {
    stateList.push_back(state);
  };

  void popState() {
    if(!stateList.empty()){
      stateList.pop_back();
    }
  };

  virtual GameState* handleInput(uint16_t inputBits) = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  Screen* getCurrentScreen() {
    return currentScreen;
  };

  void setCurrentScreen(Screen* screen) {
    currentScreen = screen; 
  };

protected:
  Screen* currentScreen;
  std::vector<GameState*> stateList;
};
#endif
