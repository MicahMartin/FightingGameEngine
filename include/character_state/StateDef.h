#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include <nlohmann/json.hpp>
#include "character_state/StateController.h"
#include "character_state/CharStateManager.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"
#include "physics/CollisionBox.h"

// TODO: Check out map usage, json.hpp is giving me the behavior I want???
class StateDef {
public:
  StateDef(int charNum, int stateNum);
  ~StateDef();

  // load shit
  void loadFlags(nlohmann::json json);
  void loadUpdate(nlohmann::json json);
  void loadAnimation(nlohmann::json json);
  void loadCollisionBoxes(nlohmann::json json);


  void enter();
  void handleInput();
  void update();
  void draw();

  enum FlagBit {
    NO_TURN = 0x01,
    FOO = 0x02,
    BAR = 0x04,
    BAZZ = 0x08,
  };

  bool checkFlag(FlagBit bit);
  int stateNum;

  // TODO: Polymorph
  std::vector<CollisionBox> collisionBoxes;
  std::vector<CollisionBox> pushBoxes;
  std::vector<CollisionBox> hurtBoxes;
  std::vector<CollisionBox> hitBoxes;

private:
  bool evalController(StateController* controller);
  bool evalCondition(std::string condition);
  void executeController(StateController* controller);

  void _changeState(std::string stateNum);
  void _velSetX(int ammount);
  void _velSetY(int ammount);
  void _moveForward(int ammount);
  void _moveBack(int ammount);
  void _moveUp(int ammount);
  void _moveDown(int ammount);
  void _setControl(int val);

  int _getAnimTime();
  int _getStateTime();
  int _getYPos();
  int _getInput(VirtualController::Input input);
  int _getStateNum();
  int _getControl();

  enum StateMethod {
    CHANGE_STATE,
    VELSET_X,
    VELSET_Y,
    MOVE_F,
    MOVE_B,
    MOVE_U,
    MOVE_D,
    SET_CONTROL
  };

  enum StateCondition {
    GET_ANIM_TIME,
    GET_STATE_TIME,
    GET_Y_POS,
    GET_INPUT,
    GET_STATE_NUM,
    GET_CONTROL
  };

  std::map<std::string, FlagBit> flagMap = {
    {"NO_TURN", NO_TURN},
    {"FOO", FOO},
    {"BAR", BAR},
    {"BAZZ", BAZZ}
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
    {"MOVE_D", MOVE_D},
    {"SET_CONTROL", SET_CONTROL}
  };

  std::map<std::string, StateCondition> stateConditionMap = {
    {"GET_ANIM_TIME", GET_ANIM_TIME},
    {"GET_STATE_TIME", GET_STATE_TIME},
    {"GET_INPUT", GET_INPUT},
    {"GET_Y_POS", GET_Y_POS},
    {"GET_STATE_NUM", GET_STATE_NUM},
    {"GET_CONTROL", GET_CONTROL}
  };

  Animation anim;
  int charNum;
  int stateTime;
  uint8_t flagByte = 0;
  std::vector<StateController> updateCommands;
  CharStateManager* charStateManager = CharStateManager::getInstance();
};
#endif
