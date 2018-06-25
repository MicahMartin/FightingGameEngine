#ifndef _gameState_h
#define _gameState_h

#include "scene.h"

class GameState{
public:
  GameState();
  virtual ~GameState();
  virtual void update();
  virtual Scene* getCurrentScene();
  virtual void setCurrentScene(Scene* scene);

private:
};
#endif /* ifndef _gameState_h */
