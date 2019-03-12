#include <bitset>
#include "states/StateManager.h"
#include "states/GameState.h"

GameState* StateManager::getState(){
  return stateStack.top();
}

void StateManager::pushState(GameState* newState){
  if(!stateStack.empty()){
    stateStack.top()->pause();
  }
  stateStack.push(newState);
  stateStack.top()->enter();
}

void StateManager::changeState(GameState* newState){
  stateStack.top()->exit();
  stateStack.pop();

  stateStack.push(newState);
  stateStack.top()->enter();

}

void StateManager::popState(){
  stateStack.top()->exit();
  stateStack.pop();
  stateStack.top()->resume();
}
