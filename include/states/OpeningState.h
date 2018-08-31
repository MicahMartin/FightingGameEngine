#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "Game.h"

class OpeningState : public GameState{
public:
  OpeningState(Game* game);
  ~OpeningState();

  void enter();
  void exit();

  void pause();
  void resume();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

private:
  Game* game;
  uint16_t controllerState;
  uint8_t stickState;
  // Scene* currentScene;
};
#endif /* ifndef _openingState_h */
