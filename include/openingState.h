#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"
#include "stateManager.h"
#include "inputManager.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();

  // gamestate
  Scene* getCurrentScene();
  void setCurrentScene(Scene* scene);

  void enter(StateManager* manager);
  void update(StateManager* manager);
  void exit();

  void pause();
  void resume();

private:
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
