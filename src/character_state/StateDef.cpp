#include "character_state/StateDef.h"
#include "game_objects/Character.h"
#include <iostream>
#include <functional>

StateDef::StateDef(int charNum) : charNum(charNum){ }
StateDef::~StateDef(){ }

void StateDef::loadControllers(nlohmann::json json){
  for(auto i : json.items()){
    StateController controller(i.value().at("condition"), i.value().at("action"));
    controllers.push_back(controller);
  }
};

void StateDef::loadAnimation(nlohmann::json json){
  anim.loadAnimEvents(json);
};

void StateDef::loadUpdate(std::string yaySetters){
  // one day I'll shake my bad java habit
  updateCommand = yaySetters;
};

void StateDef::enter(){
  anim.setAnimTime(0);
  anim.resetAnimEvents();
};

void StateDef::handleInput(){
  bool controllerMatched = false;
  for (int i = 0; i < controllers.size() && !controllerMatched; ++i) {
   controllerMatched = evalController(&controllers[i]);
   if(controllerMatched){
     // execute controller action
     std::cout << "CONTROLLER MATCHED" << std::endl;
     executeController(&controllers[i]);
   }
  }
}

void StateDef::update(){
  std::size_t pos = updateCommand.find(" ");
  if(pos != -1){
    std::string updateCall = updateCommand.substr(0, pos);
    std::string updateP = updateCommand.substr(pos+1);
    std::cout << "UPDATE CALL " << updateCall << std::endl;
    std::cout << "UPDATE PARAM " << updateP << std::endl;
    switch (stateUpdateCommandMap.at(updateCall)) {
      case MOVE_F:
        _moveForward(std::stoi(updateP));
      break;
      case MOVE_B: {
        _moveBack(std::stoi(updateP));
      break;
      }
      default:
      break;
    }
  }
}

void StateDef::draw(){
  std::pair charPos = charStateManager->getCharPointer(charNum)->getPos();
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  anim.render(charPos.first, charPos.second, faceRight);
};

bool StateDef::evalController(StateController* controller){
  std::string condition = controller->getCondition();
  std::size_t pos = condition.find(" ");
  std::string actionName = condition.substr(0, pos);
  std::string op = condition.substr(pos+1, 2);
  std::string param = condition.substr(pos+4);

  switch (stateConditionMap.at(actionName)) {
    case GET_ANIM_TIME:
      return stateOperationMap[op](_getAnimTime(), std::stoi(param));
      break;
    case GET_INPUT: {
      bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
      VirtualController::Input input = VirtualController::getInputForString(param, faceRight);
      return stateOperationMap[op](_getInput(input), 1);
      break;
    }
    default:
      return false;
  }
}

void StateDef::executeController(StateController* controller){
  std::string action = controller->getAction();
  std::size_t pos = action.find(" ");
  std::string actionFunction = action.substr(0, pos);
  std::string param = action.substr(pos+1);

  std::cout << "Executing " << actionFunction << " with param " << param << std::endl;

  switch (stateMethodMap.at(actionFunction)) {
    case CHANGE_STATE:
      _changeState(param);
      break;
  }
}

void StateDef::_changeState(std::string stateNum){
  charStateManager->getCharPointer(charNum)->changeState(std::stoi(stateNum));
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

int StateDef::_getAnimTime(){
  return anim.timeRemaining();
}

int StateDef::_getInput(VirtualController::Input input){
  return charStateManager->getCharPointer(charNum)->virtualController->isPressed(input) ? 1 : 0;
}
