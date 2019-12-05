#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include <nlohmann/json.hpp>
#include "character_state/CharStateManager.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"
#include "physics/CollisionBox.h"
#include "domain_language/Script.h"
#include "domain_language/VirtualMachine.h"


enum FlagBit {
  NO_TURN = 0x01,
  NO_TURN_ON_ENTER  = 0x02,
};

class StateDef {
public:
  StateDef(nlohmann::json::value_type json, Character* player, VirtualMachine* charVm);
  ~StateDef();

  // load shit
  void loadFlags(nlohmann::json::value_type json);
  void loadAnimation(nlohmann::json json);
  void loadCollisionBoxes(nlohmann::json json);

  bool checkFlag(FlagBit bit);

  void enter();
  void handleCancels();
  void update();
  void draw();

  void resetAnim();


  Character* player;
  VirtualMachine* charVm;
  Script updateScript;
  Script cancelScript;
  Animation anim;

  // TODO: Polymorph or atleast use a union
  std::vector<CollisionBox*> pushBoxes;
  std::vector<CollisionBox*> hurtBoxes;
  std::vector<CollisionBox*> hitBoxes;

  // TODO: Methods to talk to anim so this stuff can stay private
  int stateNum;
  int stateTime;
  bool hitboxesDisabled = false;

private:
  static std::map<std::string, FlagBit> flagMap;
  uint8_t flagByte = 0;
  CharStateManager* charStateManager = CharStateManager::getInstance();
};
#endif
