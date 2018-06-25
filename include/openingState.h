#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();
  void update();
  Scene* getCurrentScene();
  void setCurrentScene(Scene* scene);

private:
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
