#ifndef _MenuState_h
#define _MenuState_h

#include "input/VirtualController.h"
#include "states/GameState.h"
#include "Game.h"

class MenuState : public GameState{
public:
  MenuState(Game* game);
  ~MenuState();

  void enter();
  void exit();

  void pause();
  void resume();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

  // gamestate
  // Scene* getCurrentScene();
  // void setCurrentScene(Scene* scene);

private:
  Game* game;
  VirtualController* p1Controller;
  // Scene* currentScene;
};
#endif /* ifndef _openingState_h */
