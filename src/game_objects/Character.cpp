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
  virtualMachine.init(this);
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
    state.loadByteCode(i.value().at("byte_code"));
    state.loadAnimation(i.value().at("animation"));
    state.loadCollisionBoxes(i.value().at("collision_boxes"));

    stateList.push_back(state);
  }

  inputByteCode = ByteCode::compile(stateJson.at("commands"));
}

Character::~Character(){};

void Character::handleInput(){ 
  if(control){
    virtualMachine.execute(inputByteCode.data(), inputByteCode.size(), 0);
  }
};

void Character::update(){ 

  currentState->update();

  // TODO: abstract into updatePos function
  position.first += velocityX;
  position.second -= velocityY;

  if(position.second < 0){
    --velocityY;
  }

  if(position.second > 0){
    position.second = 0;
    velocityY = 0;
  }


  // TODO: abstract into updateCollisionBoxPos function
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


void Character::_changeState(int  stateNum){
  changeState(stateNum);
}

void Character::_velSetX(int ammount){
 velocityX = faceRight ? ammount : -ammount;
}

void Character::_negVelSetX(int ammount){
 velocityX = faceRight ? -ammount : ammount;
}

void Character::_velSetY(int ammount){
  velocityY = ammount;
}

void Character::_moveForward(int ammount){
  faceRight ? setX(ammount) : setX(-ammount);
}

void Character::_moveBack(int ammount){
  faceRight ? setX(-ammount) : setX(ammount);
}

void Character::_moveUp(int ammount){
  setY(ammount);
}

void Character::_moveDown(int ammount){
  setY(-ammount);
}

void Character::_setControl(int val){
  control = val;
}

void Character::_setCombo(int val){
  comboCounter = val;
}

int Character::_getAnimTime(){
  return currentState->anim.timeRemaining();
}

int Character::_getYPos(){
  int yPos = getPos().second;
  return yPos;
}

int Character::_getStateTime(){
  return currentState->stateTime;
}

int Character::_getInput(int input){
  return virtualController->isPressed(VirtualController::inputMap[input](faceRight)) ? 1 : 0;
}

int Character::_getStateNum(){
  return currentState->stateNum;
}

int Character::_getControl(){
  return control;
}

int Character::_getCombo(){
  return comboCounter;
}

int Character::_wasPressed(int input){
  VirtualController::Input inputEnum = VirtualController::inputMap[input](faceRight);
  
  return virtualController->wasPressed(VirtualController::inputMap[input](faceRight)) ? 1 : 0;
}
