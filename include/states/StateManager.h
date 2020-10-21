#ifndef _StateManager_h
#define _StateManager_h 

#include <stack>

class GameState;
class StateManager final {
public:
  static StateManager* getInstance(){
    static StateManager instance;
    return &instance;
  };
  GameState* getState();
  void pushState(GameState* newState);
  void changeState(GameState* newState);
  void popState();

  void setNetplay(bool netplaying);
  bool getNetplay();

  void setPnum(int pNum);
  int getPnum();

private:
  StateManager() = default;
  ~StateManager() = default;
  StateManager(const StateManager&) = delete;
  StateManager& operator=(const StateManager&) = delete;
  StateManager(StateManager&&) = delete;
  StateManager& operator=(StateManager&&) = delete;

  std::stack<GameState*> stateStack;
  bool netplaying;
  int pNum;
};


#endif /* ifndef _stateManager_h */
