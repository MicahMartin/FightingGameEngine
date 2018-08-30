#ifndef _menuState_h
#define _menuState_h

#include "gameState.h"
#include "game.h"
#include "virtualController.h"

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
