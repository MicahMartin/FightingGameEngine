#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"
#include "input.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();
  void update();
  void update(Input lastInput);
  Scene* getCurrentScene();
  void setCurrentScene(Scene* scene);

private:
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
