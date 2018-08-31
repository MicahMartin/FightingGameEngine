#ifndef _OpeningState_h
#define _OpeningState_h

#include "states/GameState.h"
#include "scenes/OpeningScene.h"
#include "Game.h"

class OpeningState : public GameState{
public:
  OpeningState(Game* game);
  ~OpeningState();

  void enter();
  void exit();

  void pause();
  void resume();

  GameState* handleInput(uint16_t inputBits);
  void update();
  void draw();

  Scene* getCurrentScene();
  void setCurrentScene(Scene* scene);

private:
  Game* game;
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
