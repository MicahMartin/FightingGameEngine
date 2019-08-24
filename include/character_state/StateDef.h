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
  void loadUpdate(std::string updateCommand);

  void enter();
  void handleInput();
  void update();
  void draw();

  bool evalController(StateController* controller);
  void executeController(StateController* controller);

private:
  void _changeState(std::string stateNum);
  void _moveForward(int ammount);
  void _moveBack(int ammount);
  int _getAnimTime();
  int _getInput(VirtualController::Input input);

  enum StateMethod {
    CHANGE_STATE,
  };
  enum StateCondition {
    GET_ANIM_TIME,
    GET_INPUT
  };
  enum UpdateCommand {
    MOVE_F,
    MOVE_B
  };

  std::map<std::string, std::function<bool(int, int)>> stateOperationMap = {
    {"==", std::equal_to<int>()},
    {"!=", std::not_equal_to<int>()},
    {"<<", std::less<int>()},
    {">>", std::greater<int>()},
    {">=", std::greater_equal<int>()},
    {"<=", std::less_equal<int>()},
  };

  // State methods
  std::map<std::string, StateMethod> stateMethodMap = {
    {"CHANGE_STATE", CHANGE_STATE},
  };
  std::map<std::string, StateCondition> stateConditionMap = {
    {"GET_ANIM_TIME", GET_ANIM_TIME},
    {"GET_INPUT", GET_INPUT},
  };

  std::map<std::string, UpdateCommand> stateUpdateCommandMap = {
    {"MOVE_F", MOVE_F},
    {"MOVE_B", MOVE_B}
  };

  Animation anim;
  int charNum;
  std::string updateCommand;
  std::vector<StateController> controllers;
  CharStateManager* charStateManager = CharStateManager::getInstance();
};
#endif
