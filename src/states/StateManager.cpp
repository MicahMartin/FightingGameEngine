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
  GameState* oldState = stateStack.top();
  stateStack.pop();
  oldState->exit();
  stateStack.top()->resume();
}

void StateManager::setNetplay(bool _netplaying){
  netplaying = _netplaying;
}

bool StateManager::getNetplay(){
  return netplaying;
}

void StateManager::setPnum(int pnum){
  printf("playerNum:%d\n", pnum);
  pNum = pnum;
}

int StateManager::getPnum(){
  return pNum;
}

void StateManager::setCharName(int playerNum, std::string name){
  if (playerNum == 1) {
    p1CharName = name;
  } else {
    p2CharName = name;
  }
}

std::string StateManager::getCharName(int playerNum){
  std::string charName = (playerNum == 1) ? p1CharName : p2CharName;
  return charName;
}
