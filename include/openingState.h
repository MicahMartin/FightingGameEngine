#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"
#include "virtualController.h"

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
  VirtualController* virtualController;
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
