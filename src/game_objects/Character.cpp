#include "game_objects/Character.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

Character::Character(std::pair<int, int> _position, int _playerNum) : position(_position), playerNum(_playerNum) {
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  loadStates();
  changeState(1);
}

Character::Character(std::pair<int, int> _position) : position(_position) { 
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  loadStates();
  changeState(1);
}

void Character::changeState(int stateDefNum){
  currentState = &stateList.at(stateDefNum-1);
  currentState->enter();
};

void Character::loadStates(){
  printf("Loading states\n");
  nlohmann::json stateJson;
  std::ifstream configFile("../data/characters/alucard/def.json");
  configFile >> stateJson;

  for(auto i : stateJson.items()){
    StateDef state(playerNum);
    state.loadControllers(i.value().at("controllers"));
    state.loadAnimation(i.value().at("animation"));
    stateList.push_back(state);
  }
}

Character::~Character(){};

void Character::handleInput(VirtualController* input){ 
  currentState->handleInput(input);
};

void Character::update(){ 
  // currentState->update();
};
void Character::draw(){
  currentState->draw();
};


std::pair<int,int> Character::getPos(){
  return position;
};
