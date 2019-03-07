#include <bitset>
#include "states/StateManager.h"

StateManager::StateManager() { }

StateManager::~StateManager() { 
}

GameState* StateManager::getState(){
  return stateStack.top();
}

void StateManager::pushState(GameState* newState){
  if(!stateStack.empty()){
    stateStack.top()->exit();
  }
  stateStack.push(newState);
  stateStack.top()->enter();
}

void StateManager::changeState(GameState* newState){
  // call exit on current currentState for cleanup logic
  stateStack.top()->exit();
  stateStack.pop();

  stateStack.push(newState);
  stateStack.top()->enter();

}

void StateManager::popState(){
  stateStack.top()->exit();
  stateStack.pop();
  stateStack.top()->enter();
}
