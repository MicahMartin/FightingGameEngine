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

void StateDef::enter(){
  anim.setAnimTime(0);
  anim.setAnimElemIndex(0);
};

void StateDef::handleInput(VirtualController* input){
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

void StateDef::draw(){
  std::pair charPos = charStateManager->getCharPointer(charNum)->getPos();
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  std::cout << "drawing char num " << charNum << std::endl;
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

int StateDef::_getAnimTime(){
  return anim.timeRemaining();
}
