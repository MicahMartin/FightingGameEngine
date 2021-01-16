#ifndef _VirtualController_h
#define _VirtualController_h 

#include "observer/Observer.h"
#include <list>
#include <vector>
#include <unordered_set>
#include <SDL2/SDL.h>
#include <nlohmann/json.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/serialization/access.hpp>

const int INPUT_BUFFER_MAX = 128;
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
  CONTROLLER_UP = 1,
  CONTROLLER_DOWN = 2,
  CONTROLLER_LEFT = 3,
  CONTROLLER_RIGHT = 4
} ControllerInput;

typedef enum {
  NEGATIVE = -1,
  NEUTRAL = 0,
  POSITIVE = 1
} Axis;

struct InputEvent {
  InputEvent(uint16_t inputBit, bool pressed): inputBit(inputBit), pressed(pressed){}
  InputEvent(){}
  ~InputEvent(){}

  uint16_t inputBit;
  bool pressed;
};

struct VirtualControllerObj {
  InputEvent inputHistory[INPUT_BUFFER_MAX];
  uint16_t currentState;
};

typedef std::list<InputEvent> InputFrameT;
typedef boost::circular_buffer<InputEvent> EventHistoryT;
typedef boost::circular_buffer<InputFrameT> InputHistoryT;
typedef std::vector<std::vector<InputEvent>> HistoryCopyT;

class CommandCompiler;
class VirtualController : public Observer {
public:

  VirtualController();
  ~VirtualController();

  void update();

  inline bool wasPressedWrapper(Input input, bool strict, int index, bool faceRight){
    Input relativeInput = inputEnumMap[input](faceRight);
    return wasPressed(relativeInput, strict, index);
  };
  inline bool wasReleasedWrapper(Input input, bool strict, int index, bool faceRight){
    Input relativeInput = inputEnumMap[input](faceRight);
    return wasReleased(relativeInput, strict, index);
  };
  inline bool isPressedWrapper(Input input, bool strict, int index, bool faceRight){
    strict = false;
    Input relativeInput = inputEnumMap[input](faceRight);
    return isPressed(relativeInput, strict);
  };

  void initCommandCompiler();
  bool wasPressed(Input input, bool strict = true, int index = 0, bool pressed = true);
  bool wasPressedBuffer(Input input, bool strict = true, bool pressed = true);
  bool wasReleased(Input input, bool strict = true, int index = 0);
  bool isPressed(Input input, bool strict = true);
  bool checkCommand(int commandIndex, bool faceRight);

  void setBit(Input bit);
  void clearBit(Input bit);
  void setBitOffset(uint16_t offset);
  void clearBitOffset(uint16_t offset);
  void updateAxis(bool xAxis);
  void setAxis(Input newState);
  void startCopyMode();
  void stopCopyMode();
  void playInputHistoryCopy(InputHistoryT* inputHistoryPointer);

  uint16_t getState();
  void setState(uint16_t newState);

  uint8_t getStickState();
  void printStickState();

  VirtualControllerObj saveState();
  void loadState(VirtualControllerObj state);
  void addNetInput();

  void onNotify(const char* eventName);


  static std::map<int, Input(*)(bool)> inputMap;
  static std::map<Input, Input(*)(bool)> inputEnumMap;
  static std::map<int, const char*> inputToString;

  bool debugEnabled = false;
  bool copyMode = false;
  bool playbackMode = false;
  int playbackCounter = 0;
  int copyModeSlot = 1;
  int playModeSlot = 1;
  int xAxis = NEUTRAL;
  int yAxis = NEUTRAL;
  int controllerIndex;

  InputHistoryT inputHistory;
  HistoryCopyT inputHistorySnapShot;
  std::vector<std::list<InputEvent>> inputHistoryCopy;
  std::vector<std::list<InputEvent>> inputHistoryCopyTwo;
  EventHistoryT inputEventList;
  std::vector<int> inputStateCopy;
  std::vector<uint16_t> inputStateCopyTwo;
  CommandCompiler* commandCompiler;
  HistoryCopyT wtf;
  uint16_t currentState = 0;
  uint16_t prevState = 0;

private:
  // CircularBuffer<LinkedList<InputEvent>>
};
#endif /* ifndef _virtualController_h */
