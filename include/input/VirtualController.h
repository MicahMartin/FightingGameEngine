#ifndef _VirtualController_h
#define _VirtualController_h 

#include <vector>
#include <SDL2/SDL.h>
#include "observer/Observer.h"

class VirtualController : public Observer{
public:
  VirtualController();
  ~VirtualController();

  void init();

  void update(uint16_t inputBits);

  // void setBit(uint16_t bit);
  // void clearBit(uint16_t bit);

  //void setStickState();
  //void setState(int stickState, bool addToHistory);

  uint16_t getState();
  uint8_t getStickState();
  int getInputHistorySize();

  void onNotify(const char* eventName);

  enum Input { 
    NOINPUT = 0,

    DOWN = 0x1,
    RIGHT = 0x2,
    LEFT = 0x4,
    UP = 0x8,

    DOWNLEFT = (DOWN | LEFT),
    DOWNRIGHT = (DOWN | RIGHT),
    UPLEFT = (UP | LEFT),
    UPRIGHT = (UP | RIGHT),

    LP  = 0x10,
    MP = 0x20,
    HP = 0x40,
    AP = 0x80,
    // TODO: ANYP
    // TODO: MULTIP

    LK  = 0x100,
    MK = 0x200,
    HK = 0x400,
    AK = 0x800,
    // TODO: ANYK
    // TODO: MULTIK

    START = 0x1000,
    SELECT = 0x2000,
    MISC1 = 0x4000,
    MISC2 = 0x8000,
  };

private:

  uint16_t currentState = NOINPUT;
  uint8_t stickState = NOINPUT;
  // chargeTimer[0] is down charge, chargeTimer[1] is backCharge
  int chargeTimer[1];

  // This stuff will be in the player class
};

#endif /* ifndef _virtualController_h */
