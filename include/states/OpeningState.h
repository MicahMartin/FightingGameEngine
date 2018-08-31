#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "Game.h"
#include "input/VirtualController.h"

class OpeningState : public GameState{
public:
  OpeningState(Game* game);
  ~OpeningState();

  void update();
  void draw();

  void enter();
  void exit();

  void pause();
  void resume();

  // gamestate
  // Scene* getCurrentScene();
  // void setCurrentScene(Scene* scene);

private:
  Game* game;
  VirtualController* p1Controller;
  // Scene* currentScene;
};
#endif /* ifndef _openingState_h */
