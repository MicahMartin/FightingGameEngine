#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include <nlohmann/json.hpp>
#include "character_state/StateController.h"
#include "character_state/CharStateManager.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"
#include "physics/CollisionBox.h"
#include "domain_language/ByteCode.h"


enum FlagBit {
  NO_TURN = 0x01,
  FOO = 0x02,
  BAR = 0x04,
  BAZZ = 0x08,
};

class StateDef {
public:
  StateDef(int charNum, int stateNum);
  ~StateDef();

  // load shit
  void loadFlags(nlohmann::json json);
  void loadByteCode(nlohmann::json json);
  void loadAnimation(nlohmann::json json);
  void loadCollisionBoxes(nlohmann::json json);

  bool checkFlag(FlagBit bit);

  void enter();
  void update();
  void draw();

  void resetAnim();


  std::vector<uint8_t> byteCode;
  // TODO: Polymorph
  std::vector<CollisionBox*> pushBoxes;
  std::vector<CollisionBox*> hurtBoxes;
  std::vector<CollisionBox*> hitBoxes;
  // TODO: Methods to talk to anim so this stuff can stay private
  Animation anim;
  int stateNum;
  int stateTime;

private:
  std::map<std::string, FlagBit> flagMap = {
    {"NO_TURN", NO_TURN},
    {"FOO", FOO},
    {"BAR", BAR},
    {"BAZZ", BAZZ}
  };

  int charNum;
  uint8_t flagByte = 0;
  CharStateManager* charStateManager = CharStateManager::getInstance();
};
#endif
