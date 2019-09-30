#include "game_objects/Character.h"
#include <fstream>
#include <iostream>

Character::Character(std::pair<int, int> _position, int _playerNum) {
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  maxHealth = 100;
  velocityX = 0;
  velocityY = 0;
  position = _position;
  playerNum = _playerNum;
}

Character::Character(std::pair<int, int> _position) { 
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  maxHealth = 100;
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
  std::ifstream configFile("../data/characters/alucard/def.json");
  configFile >> stateJson;

  for(auto i : stateJson.at("states").items()){
    int stateNum = i.value().at("state_num");
    StateDef state(playerNum, stateNum);

    state.loadFlags(i.value().at("flags"));
    state.loadAnimation(i.value().at("animation"));
    state.loadUpdate(i.value().at("update"));
    state.loadCollisionBoxes(i.value().at("collision_boxes"));

    stateList.push_back(state);
  }

  for(auto i : stateJson.at("commands").items()){
    StateController inputCommand(i.value().at("condition"), i.value().at("action"));
    inputCommands.push_back(inputCommand);
  }

  for(auto i : stateJson.at("hitscripts").items()){
    HitScript hitScript(i.value().at("script_num"), i.value().at("screenFreeze"), i.value().at("damage"));
    hitScripts .push_back(hitScript);
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

  if(position.second < 0){
    --velocityY;
  }

  if(position.second > 0){
    position.second = 0;
    velocityY = 0;
  }


  for (auto &cb : currentState->pushBoxes) {
    cb.positionX = position.first - (cb.width / 2);
    cb.positionY = position.second;
  }

  for (auto &cb : currentState->hurtBoxes) {
    cb.positionX = position.first + (faceRight ? cb.offsetX : - (cb.offsetX + cb.width));
    cb.positionY = position.second - cb.offsetY;
  }

  for (auto &cb : currentState->hitBoxes) {
    cb.positionX = position.first + (faceRight ? cb.offsetX : - (cb.offsetX + cb.width));
    cb.positionY = position.second - cb.offsetY;
    int stateTime = currentState->stateTime;
    if (stateTime == cb.start) {
      cb.disabled = false;
    }
    if (stateTime == cb.end) {
      cb.disabled = true;
    }
  }

};

void Character::draw(){
  // draw health bars
  currentState->draw();
};


std::pair<int,int> Character::getPos(){
  return position;
};

void Character::setXPos(int x){
  position.first = x;
};

void Character::setX(int x){
  position.first += x;
};

void Character::setY(int y){
  position.second -= y;
};
