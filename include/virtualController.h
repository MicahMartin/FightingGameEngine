#ifndef _virtualController_h
#define _virtualController_h 

#include <vector>
#include "observer.h"

//TODO: This class needs to take the input byte from inputManager and mimick a physical joystick
// compound motions need to be handled sequentially somehow??? 
// downright needs to be down, then right, but the input byte might come as down+right for the given frame
// and I wont be sure if they hit right first, then down
// maybe virtual controller should hold the input byte?
// and in the inputManager, I do virtualController->setBit(event)
//
// virtual controller could be a state machine thats fed an inputByte every frame
//
// if the virtual controller is passed a weird byte like 0110 ( left and right pressed ) or 1111 ( all directions pressed ) 
// we can mark it ( not sure what 'it' would be yet ) with a flag so we ignore it when checking for cancels

class VirtualController : public Observer{
public:
  VirtualController();
  ~VirtualController();

  void update(uint16_t inputByte);
  void setState(int stickState);
  void setState(int stickState, bool addToHistory);
  void onNotify(const char* eventName);


private:
  enum stickState {
    NOINPUT = 0,
    DOWNBACK = 1,
    DOWN = 2,
    DOWNFORWARD = 3,
    BACK = 4,
    NEUTRAL = 5,
    FORWARD = 6,
    UPBACK = 7,
    UP = 8,
    UPFORWARD = 9
  };
  // chargeTimer[0] is down charge, chargeTimer[1] is backCharge
  int chargeTimer[1];
  int currentState;
};

#endif /* ifndef _virtualController_h */
