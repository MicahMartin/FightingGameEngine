#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();

  // gamestate
  Scene* getCurrentScene();
  void setCurrentScene(Scene* scene);

  void enter(Game* manager);
  void update(Game* manager);
  void exit();

  void pause();
  void resume();

private:
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
