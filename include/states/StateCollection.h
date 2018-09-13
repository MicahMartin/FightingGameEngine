#ifndef _StateCollection_h
#define _StateCollection_h 

#include <utility>
#include "stacktrace.h"
#include "states/GameState.h"

class StateCollection {
public:
  StateCollection();
  ~StateCollection();

  void pushState(GameState* state) {
    stateList.push_back(state);
  };

  void popState() {
    if(!stateList.empty()){
      stateList.pop_back();
    }
  };

  // swap current state with the state behind it
  void swapLast(){
    std::swap(stateList.end()[-2], stateList.end()[-1]);
    print_stacktrace();
  }

  GameState* getCurrentState(){
    return stateList.back();
  }

private:
  std::vector<GameState*> stateList;
};
#endif
