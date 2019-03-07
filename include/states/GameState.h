#ifndef _GameState_h
#define _GameState_h

#include <iostream>
#include <vector>
#include "screens/Screen.h"
#include "input/VirtualController.h"

class StateManager;

class GameState{
public:
  virtual ~GameState() { };

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void handleInput(StateManager* sm, VirtualController* vc) = 0;
  virtual void update() = 0;
  virtual void draw(SDL_Renderer* renderer) = 0;

  void pause() {
    paused = true;
  }
  void resume() {
    paused = false;
  }

  Screen* getCurrentScreen() {
    return currentScreen;
  };

  void setCurrentScreen(Screen* screen) {
    currentScreen = screen; 
  };

protected:
  Screen* currentScreen;
  bool paused = false;
};
#endif
