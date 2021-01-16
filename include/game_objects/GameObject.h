#ifndef _GameObject_h
#define _GameObject_h

#include <utility>
#include <SDL_mixer.h>

enum ObjFlag {
  OF_SHOW_CB = 0x1,
};

class StateDef;
class GameObject {
public:
  virtual ~GameObject() {};

  virtual void changeState(int stateDefNum) = 0;
  virtual void cancelState(int stateDefNum) = 0;

  virtual void handleInput() = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

  virtual std::pair<int,int> getPos() = 0;
  virtual void setXPos(int x) = 0;
  virtual void setX(int x) = 0;
  virtual void setY(int y) = 0;
  virtual void updateFaceRight() = 0;
  virtual void updatePosition() = 0;
  virtual void updateCollisionBoxPositions() = 0;
  virtual void updateCollisionBoxes() = 0;
  virtual int getSoundChannel() = 0;
  virtual StateDef* getCurrentState() = 0;
  virtual Mix_Chunk* getSoundWithId(int id) = 0;
  virtual int getAnimScale() = 0;

  virtual void clearFlag(ObjFlag flag) = 0;
  virtual void setFlag(ObjFlag flag) = 0;
  virtual bool getFlag(ObjFlag flag) = 0;

  // getters for these guys
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
  int health = 0;
  int playerNum = 0;
  int velocityX = 0;
  int velocityY = 0;
  int flags = 0;
  bool faceRight = true;
  bool inputFaceRight = true;

  virtual void _changeState(int stateNum) = 0;
  virtual void _cancelState(int stateNum) = 0;
  virtual void _velSetX(int ammount) = 0;
  virtual void _negVelSetX(int ammount) = 0;
  virtual void _velSetY(int ammount) = 0;
  virtual void _moveForward(int ammount) = 0;
  virtual void _moveBack(int ammount) = 0;
  virtual void _moveUp(int ammount) = 0;
  virtual void _moveDown(int ammount) = 0;
  virtual void _setControl(int val) = 0;
  virtual void _setCombo(int val) = 0;
  virtual void _setHitStun(int val) = 0;
  virtual void _setBlockstun(int val) = 0;
  virtual void _setInstall(int val) = 0;
  virtual void _setHitCancel(int val) = 0;
  virtual void _setWhiffCancel(int val) = 0;
  virtual void _setCounter(int val) = 0;
  virtual void _setNoGravityCounter(int count) = 0;
  virtual void _setGravity(int set) = 0;
  virtual void _setAirAction(int set) = 0;
  virtual void _resetAnim() = 0;
  virtual void _activateEntity(int entityID) = 0;
  virtual void _deactivateEntity(int entityID) = 0;
  virtual void _snapToOpponent(int offset) = 0;
  virtual void _addMeter(int input) = 0;
  virtual void _subtractMeter(int input) = 0;

  virtual int _getHitStun() = 0;
  virtual int _getBlockStun() = 0;
  virtual int _getAnimTime() = 0;
  virtual int _getStateTime() = 0;
  virtual int _getYPos() = 0;
  virtual int _getVelY() = 0;
  virtual int _getVelX() = 0;
  virtual int _getStateNum() = 0;
  virtual int _getControl() = 0;
  virtual int _getCombo() = 0;
  virtual int _getIsAlive() = 0;
  virtual int _getHitCancel() = 0;
  virtual int _getWhiffCancel() = 0;
  virtual int _getAirActions() = 0;
  virtual int _getInput(int input) = 0;
  virtual int _wasPressed(int input) = 0;
  virtual int _checkCommand(int commandIndex) = 0;
  virtual int _getMeter(int meterIndex) = 0;
  virtual int _getComebackMeter() = 0;
  virtual int _getInstall() = 0;
  virtual int _getEntityStatus(int entityID) = 0;
protected:
};

#endif /* _GameObject_h */
