#ifndef _StateManager_h
#define _StateManager_h 

// not sure why i need this anymore. not gonna be passing around any 'state managers'
class GameState; 
class StateManager {
public:
  virtual void changeState(GameState* newState) = 0;
  virtual GameState* getCurrentState() = 0;
  virtual void update() = 0;
};


#endif /* ifndef _stateManager_h */
