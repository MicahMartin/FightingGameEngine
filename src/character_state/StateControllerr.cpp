#include "character_state/StateController.h"

StateController::StateController(std::string condition, std::string action) : condition(condition), action(action) { }
StateController::~StateController(){ }

std::string StateController::getCondition(){
  return condition;
};
std::string StateController::getAction(){
  return action;
};
