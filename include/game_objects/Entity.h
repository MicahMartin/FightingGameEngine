#ifndef _Entity_h
#define _Entity_h

#include <nlohmann/json.hpp>
#include <SDL_mixer.h>
#include "character_state/StateDef.h"
#include "input/VirtualController.h"
#include "domain_language/VirtualMachine.h"
#include "domain_language/Script.h"
#include "graphics/Animation.h"

class Character;
class Entity : public GameObject {
public:
  Entity(Character* owner, int entityID, const char* defPath);
  void init();

  ~Entity();

  void compileScript(const char* path, Script* script, const char* scriptTag);
  void loadStates();
  void changeState(int stateDefNum);
  void cancelState(int stateDefNum);

  void activateEntity();
  void deactivateEntity();

  void handleInput();
  void update();
  void draw();

  // position stuff
  std::pair<int,int> getPos();
  void setXPos(int x);
  void setX(int x);
  void setY(int y);
  void updateFaceRight();
  void updatePosition();
  void updateCollisionBoxPositions();
  void updateCollisionBoxes();
  StateDef* getCurrentState();
  Mix_Chunk* getSoundWithId(int id);
  int getSoundChannel();

  // getters for these guys

  int entityID;
  bool active = false;
  int width = 100;
  int control = 1;
  int hitstun = 0;
  int blockstun = 0;
  int hitStop = 0;
  int pushTime = 0;
  int maxHealth = 100;
  int hasAirAction = 0;
  int comboCounter = 0;
  int cancelPointer = 0;
  int noGravityCounter = 0;
  long frameLastAttackConnected = 0;
  bool inCorner = false;
  bool inHitStop = false;
  bool gravity = true;
  int gravityVal = 1;
  int health = 1;
  int velocityX = 0;
  int velocityY = 0;
  bool faceRight = false;
  bool inputFaceRight = false;
  bool isDead = false;

  void _changeState(int stateNum);
  void _cancelState(int stateNum);
  void _velSetX(int ammount);
  void _negVelSetX(int ammount);
  void _velSetY(int ammount);
  void _moveForward(int ammount);
  void _moveBack(int ammount);
  void _moveUp(int ammount);
  void _moveDown(int ammount);
  void _setControl(int val);
  void _setCombo(int val);
  void _setHitStun(int set);
  void _setHitCancel(int val);
  void _setWhiffCancel(int val);
  void _setNoGravityCounter(int count);
  void _setGravity(int set);
  void _setAirAction(int set);
  void _setCounter(int val);
  void _resetAnim();
  void _activateEntity(int entityID);
  void _deactivateEntity(int entityID);
  void _snapToOpponent(int offset);

  int _getHitStun();
  int _getBlockStun();
  int _getAnimTime();
  int _getStateTime();
  int _getYPos();
  int _getStateNum();
  int _getControl();
  int _getCombo();
  int _getIsAlive();
  int _getAirActions();
  int _getHitCancel();
  int _getWhiffCancel();
  int _getInput(int input);
  int _wasPressed(int input);
  int _checkCommand(int commandIndex);

  VirtualController* virtualController;
  Character* owner;
  StateDef* currentState;
  Script inputScript;
  VirtualMachine virtualMachine;
  std::vector<uint8_t> inputByteCode;

  std::vector<Animation> animList;
  std::unordered_map<int, Animation> hitSparks;
  std::unordered_map<int, Animation> guardSparks;
  std::unordered_map<int, Animation> visualEffects;

  std::vector<Mix_Chunk*> soundList;
  std::vector<Mix_Chunk*> hurtSoundList;
  bool updateFacing = false;
private:
  const char* defPath;
  nlohmann::json stateJson;
  std::vector<StateDef> stateList;
  std::pair<int, int> position;
  bool hasCommandScript = false;
  int spawnOffsetX = 0;
  int spawnOffsetY = 0;
};

#endif
