#ifndef _Game_h
#define _Game_h

#include "graphics/Graphics.h"
#include "input/InputManager.h"
#include "states/GameState.h"
#include "observer/Observer.h"


class Game : public Observer{
public:
  Game();
  ~Game();

  // state manager
  void changeState(GameState* newState);
  GameState* getCurrentState();
  void update();
   
  // observer
  void onNotify(const char* message);

  void init();
  bool stillRunning();

  // getters setters
  InputManager* getInputManager();

private:
  /* data */
  Graphics coreGraphics;
  // TODO: make this a list of input managers
  InputManager inputManager;
  GameState* gameState;
  bool running;
};
#endif /* ifndef _Game_h */
