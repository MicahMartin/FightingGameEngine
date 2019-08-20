#ifndef _StateManager_h
#define _StateManager_h 

#include <stack>
#include "GameState.h"

class StateManager {
public:
  StateManager();
  ~StateManager();

  GameState* getState();
  void pushState(GameState* newState);
  void changeState(GameState* newState);
  void popState();

private:
  std::stack<GameState*> stateStack;
};


#endif /* ifndef _stateManager_h */
