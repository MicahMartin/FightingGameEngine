#ifndef _GameObject_h
#define _GameObject_h
#include <utility>

class GameObject {
public:
  virtual ~GameObject() {};

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
  virtual void _setNoGravityCounter(int count) = 0;
  virtual void _setGravity(int set) = 0;
  virtual void _setAirAction(int set) = 0;
  virtual void _resetAnim() = 0;
  virtual void _activateEntity(int entityID) = 0;
  virtual void _deactivateEntity(int entityID) = 0;

  virtual int _getHitStun() = 0;
  virtual int _getBlockStun() = 0;
  virtual int _getAnimTime() = 0;
  virtual int _getStateTime() = 0;
  virtual int _getYPos() = 0;
  virtual int _getStateNum() = 0;
  virtual int _getControl() = 0;
  virtual int _getCombo() = 0;
  virtual int _getAirActions() = 0;
  virtual int _getInput(int input) = 0;
  virtual int _wasPressed(int input) = 0;
  virtual int _checkCommand(int commandIndex) = 0;
protected:

};

#endif /* _GameObject_h */
