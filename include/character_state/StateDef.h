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
  void loadUpdate(nlohmann::json json);

  void enter();
  void handleInput();
  void update();
  void draw();

  bool evalController(StateController* controller);
  bool evalCondition(std::string condition);
  void executeController(StateController* controller);

private:
  void _changeState(std::string stateNum);
  void _velSetX(int ammount);
  void _velSetY(int ammount);
  void _moveForward(int ammount);
  void _moveBack(int ammount);
  void _moveUp(int ammount);
  void _moveDown(int ammount);

  int _getAnimTime();
  int _getStateTime();
  int _getYPos();
  int _getInput(VirtualController::Input input);

  enum StateMethod {
    CHANGE_STATE,
    VELSET_X,
    VELSET_Y,
    MOVE_F,
    MOVE_B,
    MOVE_U,
    MOVE_D
  };

  enum StateCondition {
    GET_ANIM_TIME,
    GET_STATE_TIME,
    GET_Y_POS,
    GET_INPUT
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
    {"VELSET_X", VELSET_X},
    {"VELSET_Y", VELSET_Y},
    {"MOVE_F", MOVE_F},
    {"MOVE_B", MOVE_B},
    {"MOVE_U", MOVE_U},
    {"MOVE_D", MOVE_D}
  };

  std::map<std::string, StateCondition> stateConditionMap = {
    {"GET_ANIM_TIME", GET_ANIM_TIME},
    {"GET_STATE_TIME", GET_STATE_TIME},
    {"GET_INPUT", GET_INPUT},
    {"GET_Y_POS", GET_Y_POS},
  };

  Animation anim;
  int charNum;
  int stateTime;
  std::vector<StateController> updateCommands;
  std::vector<StateController> controllers;
  CharStateManager* charStateManager = CharStateManager::getInstance();
};
#endif
