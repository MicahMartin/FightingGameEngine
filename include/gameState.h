#ifndef _gameState_h
#define _gameState_h

#include "scene.h"
#include "stateManager.h"

class GameState{
public:

  virtual void enter(StateManager* manager) = 0;
  virtual void update(StateManager* manager) = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;

  virtual Scene* getCurrentScene() = 0;
  virtual void setCurrentScene(Scene* scene) = 0;
};
#endif
