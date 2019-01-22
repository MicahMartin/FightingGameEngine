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

  void update();

  void setBit(uint16_t bit);
  void clearBit(uint16_t bit);

  uint16_t getState();
  uint8_t getStickState();
  int getInputHistorySize();

  void onNotify(const char* eventName);

private:

  uint16_t currentState;
  uint8_t stickState;
  int counter = 0;
  std::vector<std::vector<uint16_t>> inputHistory{1};
  // This stuff will be in the player class
};

#endif /* ifndef _virtualController_h */
