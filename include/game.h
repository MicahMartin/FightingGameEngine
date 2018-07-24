#ifndef _game_h
#define _game_h

#include "graphics.h"
#include "inputManager.h"
#include "gameState.h"
#include "observer.h"


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
#endif /* ifndef _game_h */
