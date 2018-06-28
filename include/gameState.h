#ifndef _gameState_h
#define _gameState_h

#include "scene.h"
#include "input.h"

class GameState{
public:
  virtual void update() = 0;
  virtual void update(Input lastInput) = 0;
  virtual Scene* getCurrentScene() = 0;
  virtual void setCurrentScene(Scene* scene) = 0;
};
#endif /* ifndef _gameState_h */
