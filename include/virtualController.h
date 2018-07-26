#ifndef _virtualController_h
#define _virtualController_h 

#include <vector>
#include <SDL2/SDL.h>
#include "observer.h"
#include <nlohmann/json.hpp>


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

  void update();
  void init();

  void setBit(SDL_Event event);
  void clearBit(SDL_Event event);

  void setStickState();
  void setState(int stickState);
  void setState(int stickState, bool addToHistory);

  uint16_t getInputByte();
  int getInputHistorySize();
  int getState();

  void onNotify(const char* eventName);


private:
  enum InputType {  
    NEUTRAL = 0,
    DOWN = 1 << 0,
    RIGHT = 1 << 1,
    LEFT = 1 << 2,
    UP = 1 << 3,
    LIGHTP = 1 << 4,
    LIGHTK = 1 << 5,
    MEDIUMP = 1 << 6,
    MEDIUMK = 1 << 7,
    HARDP = 1 << 8,
    HARDK = 1 << 9,
    ALLP = 1 << 10,
    ALLK = 1 << 12,
    SELECT = 1 << 13,
    START = 1 << 14,
    MISC = 1 << 15,
  };

  enum stickState {
    NOINPUT,
    DL, // DOWN | LEFT
    D,  // DOWN
    DR, // DOWN | LEFT
    L,  // LEFT
    N,  // NEUTRAL  
    R,  // RIGHT
    UL, // UP | LEFT
    U,  // UP
    UR  // UP | RIGHT
  };


  uint16_t inputByte = NEUTRAL;
  // chargeTimer[0] is down charge, chargeTimer[1] is backCharge
  int chargeTimer[1];
  int currentState;

  // This stuff will be in the player class
  nlohmann::json bConf;
};

#endif /* ifndef _virtualController_h */
