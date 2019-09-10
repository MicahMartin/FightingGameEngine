#include "game_objects/Character.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

Character::Character(std::pair<int, int> _position, int _playerNum) : position(_position), playerNum(_playerNum) {
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  velocityX = 0;
  velocityY = 0;
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

  for(auto i : stateJson.at("states").items()){
    StateDef state(playerNum);
    state.loadFlags(i.value().at("flags"));
    state.loadControllers(i.value().at("controllers"));
    state.loadAnimation(i.value().at("animation"));
    state.loadUpdate(i.value().at("update"));
    stateList.push_back(state);
  }
}

Character::~Character(){};

void Character::handleInput(){ 
  currentState->handleInput();
};

void Character::update(){ 

  position.first += velocityX;
  position.second -= velocityY;

  if(position.second >> 0){
    --velocityY;
  }

  if(position.second >= 0){
    position.second = 0;
    velocityY = 0;
  }

  currentState->update();
};
void Character::draw(){
  currentState->draw();
};


std::pair<int,int> Character::getPos(){
  return position;
};

void Character::setX(int x){
  position.first += x;
};

void Character::setY(int y){
  position.second -= y;
};
