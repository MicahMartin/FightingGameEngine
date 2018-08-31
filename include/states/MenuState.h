#ifndef _MenuState_h
#define _MenuState_h

#include "states/GameState.h"
#include "Game.h"
#include "input/VirtualController.h"

class MenuState : public GameState{
public:
  MenuState(Game* game);
  ~MenuState();

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
