#ifndef _gameState_h
#define _gameState_h

#include "scene.h"

// forward decl
// not sure if this is anti pattern
// but its gotten in my way twice tonight
class Game;

class GameState{
public:
  virtual void update() = 0;
  virtual void draw() = 0;

  virtual void enter() = 0;
  virtual void exit() = 0;

  virtual void pause() = 0;
  virtual void resume() = 0;


  // virtual Scene* getCurrentScene() = 0;
  // virtual void setCurrentScene(Scene* scene) = 0;
};
#endif
