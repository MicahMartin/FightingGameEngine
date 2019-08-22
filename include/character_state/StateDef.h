#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include <nlohmann/json.hpp>
#include "character_state/StateController.h"
#include "character_state/CharStateManager.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"

class StateDef {
public:
  StateDef(int charNum);
  ~StateDef();

  void loadControllers(nlohmann::json json);
  void loadAnimation(nlohmann::json json);

  void enter();
  void handleInput(VirtualController* input);
  void draw();

  bool evalController(StateController* controller);
  void executeController(StateController* controller);
private:
  Animation anim;
  int charNum;
  std::vector<StateController> controllers;
  CharStateManager* charStateManager = CharStateManager::getInstance();
  
  enum StateMethod {
    CHANGE_STATE,
  };
  enum StateCondition {
    GET_ANIM_TIME,
  };
  // State methods
  std::map<std::string, StateMethod> stateMethodMap = {
    {"CHANGE_STATE", CHANGE_STATE},
  };
  std::map<std::string, StateCondition> stateConditionMap = {
    {"GET_ANIM_TIME", GET_ANIM_TIME},
  };

  std::map<std::string, std::function<bool(int, int)>> stateOperationMap = {
    {"==", std::equal_to<int>()},
    {"!=", std::not_equal_to<int>()},
    {"<<", std::less<int>()},
    {">>", std::greater<int>()},
    {">=", std::greater_equal<int>()},
    {"<=", std::less_equal<int>()},
  };
  void _changeState(std::string stateNum);
  int _getAnimTime();
};
#endif
