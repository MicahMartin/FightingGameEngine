#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include <nlohmann/json.hpp>
#include "game_objects/GameObject.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"
#include "physics/CollisionBox.h"
#include "domain_language/Script.h"
#include "domain_language/VirtualMachine.h"


enum FlagBit {
  NO_TURN = 0x01,
  NO_TURN_ON_ENTER  = 0x02,
  TECHABLE = 0x04,
};

struct SoundItem {
  int soundID, start;
};

class GameObject;
class StateDef {
public:
  StateDef(nlohmann::json::value_type json, VirtualMachine* charVm);
  ~StateDef();

  // load shit
  void loadFlags(nlohmann::json::value_type json);
  void loadAnimation(nlohmann::json json);
  void loadCollisionBoxes(nlohmann::json json);

  bool checkFlag(FlagBit bit);

  void enter();
  void handleCancels();
  void update();
  void draw(std::pair<int,int> position, bool faceRight, bool inHitStop);

  void resetAnim();


  VirtualMachine* charVm;
  Script updateScript;
  Script cancelScript;
  Animation anim;

  // TODO: Polymorph or atleast use a union
  std::vector<CollisionBox*> pushBoxes;
  std::vector<CollisionBox*> hurtBoxes;
  std::vector<CollisionBox*> hitBoxes;
  std::vector<CollisionBox*> throwHitBoxes;
  std::vector<CollisionBox*> throwHurtBoxes;
  std::vector<CollisionBox*> proximityBoxes;
  std::unordered_map<int, std::vector<int>> soundIndexMap;
  // TODO: Methods to talk to anim so this stuff can stay private
  int stateNum;
  int stateTime;
  int animTime;
  int techState;
  bool hitboxesDisabled = false;

  bool canWhiffCancel = false;
  bool canHitCancel = false;
  GameObject* owner;
  std::unordered_map<int, bool> hitboxGroupDisabled;

private:
  static std::map<std::string, FlagBit> flagMap;
  uint8_t flagByte = 0;
};
#endif
