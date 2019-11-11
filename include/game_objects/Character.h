#ifndef _Character_h
#define _Character_h 

#include "character_state/StateDef.h"
#include "character_state/HitScript.h"
#include "domain_language/VirtualMachine.h"
#include "domain_language/Script.h"
#include "input/VirtualController.h"
#include <nlohmann/json.hpp>

class Character {
public:
  Character(std::pair<int, int> position, int playerNum);
  Character(std::pair<int, int> position);
  void init();

  ~Character();

  void loadStates();
  void changeState(int stateDefNum);
  void cancelState(int stateDefNum);

  void handleInput();
  void update();
  void draw();

  // position stuff
  std::pair<int,int> getPos();
  void setXPos(int x);
  void setX(int x);
  void setY(int y);
  void updatePosition();
  void updateCollisionBoxes();

  // getters for these guys

  VirtualController* virtualController;
  StateDef* currentState;
  // std::vector<HitScript> hitScripts;
  std::vector<uint8_t> inputByteCode;
  Script inputScript;
  VirtualMachine virtualMachine;

  int control = 1;
  int comboCounter = 0;
  int playerNum;
  int velocityX;
  int velocityY;
  int health;
  int maxHealth;
  bool faceRight;
  bool gravity = true;
  int noGravityCounter = 0;
  int cancelPointer = 0;
  int hitstun = 0;
  int hasAirAction = 0;

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
  void _setNoGravityCounter(int count);
  void _setGravity(int set);
  void _setAirAction(int set);
  void _resetAnim();

  int _getHitStun();
  int _getAnimTime();
  int _getStateTime();
  int _getYPos();
  int _getStateNum();
  int _getControl();
  int _getCombo();
  int _getAirActions();
  int _getInput(int input);
  int _wasPressed(int input);
  int _checkCommand(int commandIndex);


private:
  nlohmann::json stateJson;
  std::vector<StateDef> stateList;
  std::pair<int, int> position;
};

#endif
