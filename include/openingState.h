#ifndef _openingState_h
#define _openingState_h

#include "gameState.h"

class OpeningState : public GameState{
public:
  OpeningState();
  ~OpeningState();
  void update();

private:
  Scene* currentScene;
};
#endif /* ifndef _openingState_h */
