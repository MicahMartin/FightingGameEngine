#ifndef _game_h
#define _game_h

#include "graphics.h"
#include "inputManager.h"
#include "gameState.h"
#include "observer.h"


class Game : public Observer, StateManager{
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

private:
  /* data */
  Graphics coreGraphics;
  InputManager inputManager;
  GameState* gameState;
  bool running;
};
#endif /* ifndef _game_h */
