#ifndef _Character_h
#define _Character_h 

#include <nlohmann/json.hpp>
#include <SDL_mixer.h>
#include "game_objects/GameObject.h"
#include "character_state/StateDef.h"
#include "domain_language/VirtualMachine.h"
#include "domain_language/Script.h"
#include "input/VirtualController.h"
#include "graphics/Animation.h"
#include "game_objects/VisualEffect.h"
#include "game_objects/Entity.h"

struct CharStateObj {
  int control;
  int hitstun;
  int blockstun;
  int hitStop;
  int pushTime;
  int pushBackVelocity;
  int comebackCounter;
  int hasAirAction;
  int comboCounter;
  int cancelPointer;
  int noGravityCounter;
  int velocityX;
  int momentum;
  int mass;
  int velocityY;
  int health;
  int redHealth;
  int redHealthCounter;
  int gravityVal;
  int hitPushTime;
  int hitPushVelX;
  int hitPushVelY;
  int meter;
  int comeback;
  int currentHurtSoundID;
  int soundChannel;
  int flashCounter;
  int auraID;
  int positionX;
  int positionY;
  long frameLastAttackConnected;
  bool inCorner;
  bool inHitStop;
  bool gravity;
  bool isDead;
  bool faceRight;
  bool inputFaceRight;
  bool isRed;
  bool isGreen;
  bool isLight;
  bool installMode;
  bool auraActive;
  int currentState;

  StateDefObj stateDefObj;
  EntityStateObj entityStates[3];
  std::string inputHistoryArc;
  // std::unordered_map<int, EntityStateObj> entityStates;
};

typedef enum {
  SS_IDLE = 1,
  SS_WALK_F,
  SS_WALK_B,
  SS_CROUCH,
  SS_JUMP_N = 5,
  SS_JUMP_F,
  SS_JUMP_B,
  SS_HURT,
  SS_HURT_RECOVERY,
  SS_AIR_HURT = 10,
  SS_AIR_HURT_RECOVERY,
  SS_KNOCKDOWN,
  SS_BLOCK_STAND,
  SS_BLOCK_CROUCH,
  SS_BLOWBACK_FALLING = 15,
  SS_AIR_BLOCK,
  SS_PRE_MATCH,
  SS_DEAD_STANDING,
  SS_DEAD_KNOCKDOWN,
  SS_THROW_TECH = 20,
  SS_AIR_THROW_TECH,
  SS_PUSH_BLOCK,
  SS_CROUCH_PUSH_BLOCK,
  SS_GROUNDBOUNCE_FLING,
  SS_GROUNDBOUNCE_IMPACT = 25,
  SS_AIR_PUSH_BLOCK,
  SS_JUMP_R,
  SS_AIR_TECH,
  SS_DEAD_FALLING,
  SS_FLOAT_HURT = 30,
  SS_FLOAT_HURT_RECOVERY,
  SS_FORWARD_THROW,
  SS_FORWARD_THROW_ATTEMPT,
  SS_FORWARD_THROW_SUCCESS
} SpecialState;

class Character : public GameObject {
public:
  Character(std::pair<int, int> position, int playerNum);
  Character(std::pair<int, int> position);
  void init(const char* defPath);

  ~Character();

  void compileScript(const char* path, Script* script, const char* scriptTag);
  void loadStates(const char* path);
  void loadCustomStates(const char* path);
  void refresh();
  void changeState(int stateDefNum);
  void cancelState(int stateDefNum);
  void setCurrentState(int stateDefNum);

  void handleInput();
  void update();
  void draw();

  CharStateObj saveState();
  void loadState(CharStateObj stateObj);
  int stateCount = 0;

  // position stuff
  std::pair<int,int> getPos();
  void setXPos(int x);
  void setYPos(int y);
  void setX(int x);
  void setY(int y);
  void updateFaceRight();
  void updatePosition();
  void updateCollisionBoxPositions();
  void updateCollisionBoxes();
  void activateVisFX(int visID);
  int getSoundChannel();
  int getAnimScale();
  bool hurtState(int state);
  bool blockState(int state);
  StateDef* getCurrentState();
  Mix_Chunk* getSoundWithId(int id);

  void clearFlag(ObjFlag flag);
  void setFlag(ObjFlag flag);
  bool getFlag(ObjFlag flag);

  // getters for these guys
  int width = 10000;
  int maxHealth = 100;
  int maxMeter = 1000;
  int maxComeback = 1000;
  int hurtSoundMax = 3;

  int control = 1;
  int hitstun = 0;
  int blockstun = 0;
  int hitStop = 0;
  int pushTime = 0;
  int pushBackVelocity = 0;
  int comebackCounter = 30;
  int hasAirAction = 0;
  int comboCounter = 0;
  int cancelPointer = 0;
  int noGravityCounter = 0;
  long frameLastAttackConnected = 0;
  bool inCorner = false;
  bool inHitStop = false;
  bool gravity = true;
  int gravityVal = 1;
  bool isDead = false;
  int velocityX = 0;
  int momentum = 0;
  int mass = 1;
  int velocityY = 0;
  int velocityMinimumY = 0;
  int velocityMaximumY = 0;
  int velocityMinimumX = 0;
  int velocityMaximumX = 0;
  int health = 100;
  int redHealth = 100;
  int redHealthCounter = 0;
  int playerNum;
  bool faceRight;
  bool inputFaceRight;
  int hitPushTime = 0;
  int hitPushVelX = 0;
  int hitPushVelY = 0;
  int meter = 0;
  int comeback = 750;
  int installCounter = 0;
  int currentHurtSoundID = 1;
  int soundChannel = 0;
  int flashCounter = 0;
  float animScale = 4;
  bool isRed = false;
  bool isGreen = false;
  bool isLight = false;
  bool installMode = false;
  bool auraActive = false;
  int auraID = 0;
  std::string charName = "";
  std::pair<int, int> position;

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
  void _setHitStun(int val);
  void _setBlockstun(int val);
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
  void _addMeter(int input);
  void _subtractMeter(int input);
  void _setInstall(int input);

  int _getHitStun();
  int _getBlockStun();
  int _getAnimTime();
  int _getStateTime();
  int _getHitCancel();
  int _getWhiffCancel();
  int _getYPos();
  int _getVelY();
  int _getVelX();
  int _getStateNum();
  int _getControl();
  int _getCombo();
  int _getAirActions();
  int _getIsAlive();
  int _getInput(int input);
  int _wasPressed(int input);
  int _checkCommand(int commandIndex);
  int _getMeter();
  int _getComebackMeter();
  int _getEntityStatus(int entityID);
  int _getInstall();

  VirtualController* virtualController;
  StateDef* currentState;
  Character* otherChar;
  Script inputScript;
  VirtualMachine virtualMachine;
  std::vector<uint8_t> inputByteCode;
  std::vector<Entity> entityList;

  std::unordered_map<int, VisualEffect> hitSparks;
  std::unordered_map<int, VisualEffect> guardSparks;
  std::unordered_map<int, VisualEffect> visualEffects;

  std::unordered_map<int, SoundObj> soundsEffects;
  std::unordered_map<int, SoundObj> hurtSoundEffects;
  std::unordered_map<SpecialState, int> specialStateMap;
private:
  nlohmann::json stateJson;
  std::vector<StateDef> stateList;
};

#endif
