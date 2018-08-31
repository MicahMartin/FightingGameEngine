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

  void init();
  void update();

  // state manager
  void changeState(GameState* newState);
  GameState* getCurrentState();

   
  // observer
  void onNotify(const char* message);

  // getters setters
  bool stillRunning();
  InputManager* getInputManager();
  Graphics* getGraphics();

private:
  /* data */
  Graphics coreGraphics;
  InputManager inputManager;
  GameState* currentState;
  bool running;
};
#endif /* ifndef _Game_h */
