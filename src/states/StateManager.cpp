#include <bitset>
#include "states/StateManager.h"

StateManager::StateManager() { }

StateManager::~StateManager() { 
}

GameState* StateManager::getState(){
  return stateStack.top();
}

void StateManager::pushState(GameState* newState){
  newState->enter();
  stateStack.push(newState);
}

void StateManager::changeState(GameState* newState){
  // call exit on current currentState for cleanup logic
  popState();
  pushState(newState);
}

void StateManager::popState(){
  getState()->exit();
  stateStack.pop();
}
