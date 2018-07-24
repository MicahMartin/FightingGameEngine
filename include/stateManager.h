#ifndef _stateManager_h
#define _stateManager_h 

class GameState; 
class StateManager {
public:
  virtual void changeState(GameState* newState) = 0;
  virtual GameState* getCurrentState() = 0;
  virtual void update() = 0;
};


#endif /* ifndef _stateManager_h */
