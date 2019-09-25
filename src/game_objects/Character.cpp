#include "game_objects/Character.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

Character::Character(std::pair<int, int> _position, int _playerNum) {
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  velocityX = 0;
  velocityY = 0;
  position = _position;
  playerNum = _playerNum;
}

Character::Character(std::pair<int, int> _position) { 
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  position = _position;
}

void Character::init(){
  loadStates();
  changeState(1);
}

void Character::changeState(int stateDefNum){
  currentState = &stateList.at(stateDefNum-1);
  currentState->enter();
};

void Character::loadStates(){
  printf("%d Loading states\n", playerNum);
  nlohmann::json stateJson;
  std::ifstream configFile("../data/characters/alucard/def.json");
  configFile >> stateJson;

  for(auto i : stateJson.at("states").items()){
    StateDef state(playerNum);

    state.loadFlags(i.value().at("flags"));
    state.loadAnimation(i.value().at("animation"));
    state.loadUpdate(i.value().at("update"));
    state.loadCollisionBoxes(i.value().at("collision_boxes"));

    stateList.push_back(state);
  }
}

Character::~Character(){};

void Character::handleInput(){ 
  currentState->handleInput();
};

void Character::update(){ 

  currentState->update();

  position.first += velocityX;
  position.second -= velocityY;

  if(position.second >> 0){
    --velocityY;
  }

  if(position.second >= 0){
    position.second = 0;
    velocityY = 0;
  }


  for (auto &cb : currentState->pushBoxes) {
    cb.positionX = position.first - (cb.width / 2);
    cb.positionY = position.second;
  }

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
