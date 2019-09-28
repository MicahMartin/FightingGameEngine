#include <iostream>
#include <functional>
#include "character_state/StateDef.h"
#include "game_objects/Character.h"
#include "physics/CollisionBox.h"
#include "Util.h"

StateDef::StateDef(int charNum, int stateNum) : charNum(charNum), stateNum(stateNum){ }
StateDef::~StateDef(){ }

void StateDef::loadFlags(nlohmann::json json){
  for(auto i : json.items()){
    std::string flag(i.value());
    flagByte |= flagMap[flag];
  }
};

void StateDef::loadAnimation(nlohmann::json json){
  anim.loadAnimEvents(json);
};

void StateDef::loadUpdate(nlohmann::json json){
  for(auto i : json.items()){
    StateController updateCommand(i.value().at("condition"), i.value().at("action"));
    updateCommands.push_back(updateCommand);
  }
};

void StateDef::loadCollisionBoxes(nlohmann::json json){
  for(auto i : json.items()){
    CollisionBox::CollisionType type = CollisionBox::collisionTypeMap.at(i.value().at("type"));
    int width = i.value().at("width");
    int height = i.value().at("height");
    int offsetX = i.value().at("offsetX");
    int offsetY = i.value().at("offsetY");
    int start = i.value().at("start");
    int end = i.value().at("end");

    Character* player = charStateManager->getCharPointer(charNum);
    printf("%d loading collision boxes \n", player->playerNum);
    std::pair charPos = player->getPos();

    CollisionBox cb(type, width, height, offsetX, offsetY, start, end);
    cb.positionX = charPos.first - (width/2);
    cb.positionY = charPos.second;

    switch (type) {
      case CollisionBox::POSITION:
        pushBoxes.push_back(cb);
        break;
      case CollisionBox::HURT:
        hurtBoxes.push_back(cb);
        break;
      case CollisionBox::HIT:
        hitBoxes.push_back(cb);
        break;
    }
  }
};

void StateDef::enter(){
  anim.setAnimTime(0);
  anim.resetAnimEvents();
  stateTime = 0;
};

void StateDef::handleInput(){
  std::vector<StateController> inputCommands = charStateManager->getCharPointer(charNum)->inputCommands;
  
  bool commandMatched = false;
  for (int i = 0; i < inputCommands.size() && !commandMatched; ++i) {
  std::string condition = inputCommands[i].getCondition();
   commandMatched = evalController(&inputCommands[i]);
   if(commandMatched){
     // execute controller action
     executeController(&inputCommands[i]);
   }
  }
}

void StateDef::update(){
  for(int i = 0; i < updateCommands.size(); ++i){
    bool updateCondition = evalController(&updateCommands[i]);
    if(updateCondition){
      executeController(&updateCommands[i]);
    }
  }
  stateTime++;
}

void StateDef::draw(){
  std::pair charPos = charStateManager->getCharPointer(charNum)->getPos();
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  anim.render(charPos.first, charPos.second, faceRight);

  // stateTime is 2
  for (auto cb : pushBoxes) {
    if(cb.end == -1 || ( cb.start >= stateTime && cb.end <= stateTime )){
      cb.render();
    }
  }
  for (auto cb : hurtBoxes) {
    if(cb.end == -1 || ( stateTime >= cb.start && stateTime <= cb.end )){
      cb.render();
    }
  }
  for (auto cb : hitBoxes) {
    if(cb.end == -1 || ( stateTime >= cb.start && stateTime <= cb.end )){
      cb.render();
    }
  }
};

bool StateDef::checkFlag(StateDef::FlagBit bit){
  return flagByte & bit;
};

bool StateDef::evalController(StateController* controller){
  std::string condition = controller->getCondition();

  std::size_t hasAndCondition = condition.find("&&");
  if (hasAndCondition != -1) {
    std::vector<std::string> conditions = Util::split(condition, "&&");
    bool finalFlag = false;

    for (auto andCondition : conditions) {
      finalFlag = evalCondition(andCondition);
      if (!finalFlag) {
        return false;
      }
    }
    return finalFlag;
  }

  return evalCondition(condition);
}

bool StateDef::evalCondition(std::string condition){
  std::size_t pos = condition.find(" ");
  if(pos == -1){
    return true;
  }
  std::string actionName = condition.substr(0, pos);
  std::string op = condition.substr(pos+1, 2);
  std::string param = condition.substr(pos+4);

  switch (stateConditionMap.at(actionName)) {
    case GET_ANIM_TIME:
      return stateOperationMap[op](_getAnimTime(), std::stoi(param));
      break;
    case GET_STATE_TIME:
      return stateOperationMap[op](_getStateTime(), std::stoi(param));
      break;
    case GET_Y_POS:
      return stateOperationMap[op](_getYPos(), std::stoi(param));
      break;
    case GET_INPUT: {
      bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
      VirtualController::Input input = VirtualController::inputMap[param](faceRight);
      return stateOperationMap[op](_getInput(input), 1);
      break;
    }
    case WAS_PRESSED: {
      bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
      VirtualController::Input input = VirtualController::inputMap[param](faceRight);
      return stateOperationMap[op](_wasPressed(input), 1);
      break;
    }
    case GET_STATE_NUM:
      return stateOperationMap[op](_getStateNum(), std::stoi(param));
      break;
    case GET_CONTROL:
      return stateOperationMap[op](_getControl(), std::stoi(param));
      break;
  }
}

void StateDef::executeController(StateController* controller){
  std::string action = controller->getAction();
  std::size_t pos = action.find(" ");
  std::string actionFunction = action.substr(0, pos);
  std::string param = action.substr(pos+1);

  switch (stateMethodMap.at(actionFunction)) {
    case CHANGE_STATE:
      _changeState(param);
      break;
    case VELSET_X:
      _velSetX(std::stoi(param));
      break;
    case VELSET_Y:
      _velSetY(std::stoi(param));
      break;
    case MOVE_F:
      _moveForward(std::stoi(param));
      break;
    case MOVE_B:
      _moveBack(std::stoi(param));
      break;
    case MOVE_U:
      _moveUp(std::stoi(param));
      break;
    case MOVE_D:
      _moveDown(std::stoi(param));
      break;
    case SET_CONTROL:
      _setControl(std::stoi(param));
      break;
  }
}

void StateDef::_changeState(std::string stateNum){
  charStateManager->getCharPointer(charNum)->changeState(std::stoi(stateNum));
}
void StateDef::_velSetX(int ammount){
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  faceRight ? charStateManager->getCharPointer(charNum)->velocityX = ammount : charStateManager->getCharPointer(charNum)->velocityX = -ammount;
}

void StateDef::_velSetY(int ammount){
  charStateManager->getCharPointer(charNum)->velocityY = ammount;
}

void StateDef::_moveForward(int ammount){
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  if (faceRight) {
    charStateManager->getCharPointer(charNum)->setX(ammount);
  } else {
    charStateManager->getCharPointer(charNum)->setX(-ammount);
  }
}

void StateDef::_moveBack(int ammount){
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  if (faceRight) {
    charStateManager->getCharPointer(charNum)->setX(-ammount);
  } else {
    charStateManager->getCharPointer(charNum)->setX(ammount);
  }
}

void StateDef::_moveUp(int ammount){
  charStateManager->getCharPointer(charNum)->setY(ammount);
}

void StateDef::_moveDown(int ammount){
  charStateManager->getCharPointer(charNum)->setY(-ammount);
}

void StateDef::_setControl(int val){
  charStateManager->getCharPointer(charNum)->control = val;
}

int StateDef::_getAnimTime(){
  return anim.timeRemaining();
}

int StateDef::_getYPos(){
  int yPos = charStateManager->getCharPointer(charNum)->getPos().second;
  return yPos;
}

int StateDef::_getStateTime(){
  return stateTime;
}

int StateDef::_getInput(VirtualController::Input input){
  return charStateManager->getCharPointer(charNum)->virtualController->isPressed(input) ? 1 : 0;
}

int StateDef::_getStateNum(){
  return charStateManager->getCharPointer(charNum)->currentState->stateNum;
}

int StateDef::_getControl(){
  int control = charStateManager->getCharPointer(charNum)->control;
  return control;
}

int StateDef::_wasPressed(VirtualController::Input input){
  return charStateManager->getCharPointer(charNum)->virtualController->wasPressed(input) ? 1 : 0;
}
