#ifndef _gameState_h
#define _gameState_h

#include "scene.h"

class GameState{
public:
  virtual void update();
  virtual Scene* getCurrentScene();
  virtual void setCurrentScene(Scene* scene);
};
#endif /* ifndef _gameState_h */
