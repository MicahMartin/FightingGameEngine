#ifndef _gameState_h
#define _gameState_h

#include "scene.h"

class GameState{
public:
  GameState();
  ~GameState();
  void update();

private:
  /* data */
  Scene currentScene;
};
#endif /* ifndef _gameState_h */
