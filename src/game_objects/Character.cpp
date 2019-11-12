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
  virtualMachine.character = this;
  loadStates();
  changeState(1);
}

void Character::changeState(int stateDefNum){
  cancelPointer = 0;
  currentState = &stateList.at(stateDefNum-1);
  currentState->enter();
};

void Character::cancelState(int stateDefNum){
  cancelPointer = stateDefNum;
};

// NOTE: must delete
static inline char* readFile(const char* path){
  // why is everything in c / c++ a chore..
  std::ifstream file(path, std::ios::in|std::ios::binary|std::ios::ate);
  if(file.is_open()){

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    char* fileContent = new char[size];

    file.seekg(0, std::ios::beg);
    file.read(fileContent, size);
    file.close();

    fileContent[size] = '\0';
    // printf("read file: %s \n %s\n", path, fileContent);

    return fileContent;
  } else {
    throw std::runtime_error("unable to open file!");
  }
}

void Character::loadStates(){
  printf("%d Loading states\n", playerNum);
  std::ifstream configFile("../data/characters/alucard/def.json");
  configFile >> stateJson;

  // compile character's input scripte 
  char* inputCommandSource = readFile(stateJson.at("command_script").get<std::string>().c_str());
  if(!virtualMachine.compiler.compile(inputCommandSource, &inputScript, "inputCommandScript")){
    inputScript.disassembleScript("input command script");
    throw std::runtime_error("inputScript failed to compile");
  }
  delete inputCommandSource;

  for(auto i : stateJson.at("states").items()){
    int stateNum = i.value().at("state_num");
    std::string scriptTag = "updateScript:" + std::to_string(stateNum);
    StateDef state(stateNum, this, &virtualMachine);

    state.loadFlags(i.value().at("flags"));
    // compile state's update script%s
    char* stateUpdateSource = readFile(i.value().at("update_script").get<std::string>().c_str());
    if(!virtualMachine.compiler.compile(stateUpdateSource, &state.updateScript, scriptTag.c_str())){
      printf("looking at %s\n", scriptTag.c_str());
      throw std::runtime_error("updateScript failed to compile");
    }
    delete stateUpdateSource;

    try {
      // compile state's cancel script
      char* cancelSource = readFile(i.value().at("cancel_script").get<std::string>().c_str());
      std::string cancelScriptTag = "cancelScript:" + std::to_string(stateNum);
      if(!virtualMachine.compiler.compile(cancelSource, &state.cancelScript, cancelScriptTag.c_str())){
        throw std::runtime_error("cancelScript failed to compile");
      }
      delete cancelSource;
    } catch(std::runtime_error e){
    } catch(nlohmann::json::exception e) { 

    }
    state.loadAnimation(i.value().at("animation"));
    state.loadCollisionBoxes(i.value().at("collision_boxes"));

    stateList.push_back(state);
  }
}

Character::~Character(){};

void Character::handleInput(){ 
  if (hitstun > 0) {
    hitstun--;
  }
  if(cancelPointer != 0){
    changeState(cancelPointer);
  }
  if(control){
    // TODO: Precompile all scripts
    virtualMachine.execute(&inputScript);
  }
};

void Character::update(){ 
  currentState->update();
  updatePosition();
  updateCollisionBoxes();
};

void Character::updatePosition(){
  position.first += velocityX;
  position.second -= velocityY;

  if(noGravityCounter > 0){
    gravity = false;
    if(--noGravityCounter == 0){
      gravity = true;
    }
  };
  if(position.second < 0 && gravity){
    --velocityY;
  }

  if(position.second > 0){
    position.second = 0;
    velocityY = 0;
  }
}

void Character::updateCollisionBoxes(){
  // TODO: abstract into updateCollisionBoxPos function
  int stateTime = currentState->stateTime;
  for (auto cb : currentState->pushBoxes) {
    cb->positionX = position.first - (cb->width / 2);
    cb->positionY = position.second;
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (cb->end == -1 || stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->hurtBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (cb->end == -1 || stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->hitBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

}

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

void Character::_cancelState(int  stateNum){
  cancelState(stateNum);
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

void Character::_setGravity(int on){
  gravity = on;
}

void Character::_setNoGravityCounter(int count){
  noGravityCounter = count;
}

void Character::_resetAnim(){
  currentState->resetAnim();
}


int Character::_getAnimTime(){
  return currentState->anim.timeRemaining();
}

int Character::_getYPos(){
  int yPos = abs(getPos().second);
  return yPos;
}

int Character::_getStateTime(){
  return currentState->stateTime;
}

int Character::_getInput(int input){
  Input inputType = VirtualController::inputMap[input](faceRight);
  return virtualController->isPressed(inputType) ? 1 : 0;
}

int Character::_getStateNum(){
  return currentState->stateNum;
}

int Character::_getControl(){
  return control;
}

int Character::_getAirActions(){
  return hasAirAction;
}

void Character::_setAirAction(int operand){
  hasAirAction = operand;
}

int Character::_getCombo(){
  return comboCounter;
}

int Character::_wasPressed(int input){
  return virtualController->wasPressed(VirtualController::inputMap[input](faceRight), 0) ? 1 : 0;
}

int Character::_getHitStun(){
  return hitstun;
}

int Character::_checkCommand(int commandIndex){
  return virtualController->checkCommand(commandIndex, faceRight);
}
