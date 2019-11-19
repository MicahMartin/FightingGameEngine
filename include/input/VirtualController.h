#ifndef _VirtualController_h
#define _VirtualController_h 

#include "observer/Observer.h"
#include "input/CommandCompiler.h"
#include <list>
#include <vector>
#include <unordered_set>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>
#include <boost/circular_buffer.hpp>

typedef enum { 
  NOINPUT = 0,

  RIGHT = 0x1,
  LEFT = 0x2,
  UP = 0x4,
  DOWN = 0x8,

  LP  = 0x10,
  MP = 0x20,
  HP = 0x40,
  AP = 0x80,

  LK  = 0x100,
  MK = 0x200,
  HK = 0x400,
  AK = 0x800,

  START = 0x1000,
  SELECT = 0x2000,
  MISC1 = 0x4000,
  MISC2 = 0x8000,

  DOWNLEFT = (DOWN | LEFT),
  DOWNRIGHT = (DOWN | RIGHT),
  UPLEFT = (UP | LEFT),
  UPRIGHT = (UP | RIGHT),
} Input;

typedef enum {
  NEGATIVE = -1,
  NEUTRAL = 0,
  POSITIVE = 1
} Axis;

struct InputEvent {
  uint16_t inputBit;
  bool pressed;
  InputEvent(uint16_t inputBit, bool pressed): inputBit(inputBit), pressed(pressed){}
};


class VirtualController : public Observer {
public:

  VirtualController();
  ~VirtualController();

  void update();

  bool wasPressed(Input input, int index, bool strict = true, bool pressed = true);
  bool wasReleased(Input input, int index, bool strict = true);
  bool isPressed(Input input, bool strict = true);
  bool checkCommand(int commandIndex, bool faceRight);

  void setBit(uint16_t bit);
  void clearBit(uint16_t bit);
  void setBitOffset(uint16_t offset);
  void clearBitOffset(uint16_t offset);
  void updateAxis(bool xAxis);

  uint16_t getState();
  uint8_t getStickState();
  void printStickState();
  void onNotify(const char* eventName);


  static std::map<int, Input(*)(bool)> inputMap;
  static std::map<Input, const char*> inputToString;

  bool debugEnabled = false;
  int xAxis = NEUTRAL;
  int yAxis = NEUTRAL;
  boost::circular_buffer<InputEvent> inputEventList;
  CommandCompiler commandCompiler;
private:
  uint16_t currentState = 0;
  boost::circular_buffer<std::list<InputEvent>> inputHistory;
};
#endif /* ifndef _virtualController_h */
