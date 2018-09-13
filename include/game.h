#ifndef _Game_h
#define _Game_h

#include "graphics/Graphics.h"
#include "input/InputManager.h"
#include "observer/Observer.h"
#include "states/GameState.h"


class Game : public Observer{
public:
  Game();
  ~Game();

  void init();
  void update();

  // state manager
  void changeState(GameState* newState);
  GameState* getCurrentState();

  void pushState(GameState* state) {
    stateList.push_back(state);
  };

  void popState() {
    if(!stateList.empty()){
      stateList.pop_back();
    }
  };


   
  // observer
  void onNotify(const char* message);

  // getters setters
  bool stillRunning();
  InputManager* getInputManager();
  Graphics* getGraphics();

private:
  Graphics coreGraphics;
  InputManager inputManager;
  std::vector<GameState*> stateList;
  GameState* currentState;
  bool running;
  int gameTime = 0;
};
#endif
