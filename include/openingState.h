#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"
#include "inputManager.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();
  void update();
  void update(InputManager* inputManager);
  Scene* getCurrentScene();
  void setCurrentScene(Scene* scene);

private:
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
