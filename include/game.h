#ifndef _Game_h
#define _Game_h

#include "graphics/Graphics.h"
#include "input/InputManager.h"
#include "observer/Observer.h"
#include "states/GameState.h"
#include "states/StateCollection.h"


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
  Graphics coreGraphics;
  InputManager inputManager;
  std::vector<StateCollection*> stateList;
  GameState* currentState;
  bool running;
  int gameTime = 0;
};
#endif
