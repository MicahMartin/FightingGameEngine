#include "input/VirtualController.h"
#include "input/CommandCompiler.h"
#include <bitset>
#include <iostream>
#include <fstream>
#include <boost/asio/streambuf.hpp>
#include <nlohmann/json.hpp>

// include input and output archivers
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

// include this header to serialize vectors
#include <boost/serialization/vector.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/binary_object.hpp>

std::map<int, Input(*)(bool)> VirtualController::inputMap = {
  {1, [](bool faceRight){return faceRight ? DOWNLEFT:DOWNRIGHT;}},
  {2, [](bool faceRight){return DOWN;}},
  {3, [](bool faceRight){return faceRight ? DOWNRIGHT:DOWNLEFT;}},
  {4, [](bool faceRight){return faceRight ? LEFT:RIGHT;}},
  {5, [](bool faceRight){return NOINPUT;}},
  {6, [](bool faceRight){return faceRight ? RIGHT:LEFT;}},
  {7, [](bool faceRight){return faceRight ? UPLEFT:UPRIGHT;}},
  {8, [](bool faceRight){return UP;}},
  {9, [](bool faceRight){return faceRight ? UPRIGHT:UPLEFT;}},
  {10, [](bool faceRight){return LP;}},
  {11, [](bool faceRight){return LK;}},
  {12, [](bool faceRight){return MP;}},
  {13, [](bool faceRight){return MK;}},
};

std::map<Input, Input(*)(bool)> VirtualController::inputEnumMap = {
  {RIGHT, [](bool faceRight){return faceRight ? RIGHT:LEFT;}},
  {LEFT, [](bool faceRight){return faceRight ? LEFT:RIGHT;}},
  {DOWNRIGHT, [](bool faceRight){return faceRight ? DOWNRIGHT:DOWNLEFT;}},
  {DOWNLEFT, [](bool faceRight){return faceRight ? DOWNLEFT:DOWNRIGHT;}},
  {UPRIGHT, [](bool faceRight){return faceRight ? UPRIGHT:UPLEFT;}},
  {UPLEFT, [](bool faceRight){return faceRight ? UPLEFT:UPRIGHT;}},
  {NOINPUT, [](bool faceRight){return NOINPUT;}},
  {UP, [](bool faceRight){return UP;}},
  {DOWN, [](bool faceRight){return DOWN;}},
  {LP, [](bool faceRight){return LP;}},
  {LK, [](bool faceRight){return LK;}},
  {MP, [](bool faceRight){return MP;}},
  {MK, [](bool faceRight){return MK;}},
};

std::map<int, const char*> VirtualController::inputToString = {
  {NOINPUT, "NEUTRAL"},
  {DOWN, "DOWN"},
  {RIGHT, "RIGHT"},
  {LEFT, "LEFT"},
  {UP, "UP"},
  {DOWNLEFT, "DOWNLEFT"},
  {DOWNRIGHT, "DOWNRIGHT"},
  {UPLEFT, "UPLEFT"},
  {UPRIGHT, "UPRIGHT"},
  {LP, "LIGHT_P"},
  {LK, "LIGHT_K"},
  {MP, "MEDIUM_P"},
  {MK, "MEDIUM_K"},
};

VirtualController::VirtualController() : inputHistorySnapShot(128) { 
  inputHistory.set_capacity(128);
  inputEventList.set_capacity(10);
  inputHistoryCopy.reserve(128);

  commandCompiler = new CommandCompiler();
  commandCompiler->controllerPointer = this;
}

VirtualController::~VirtualController() { 
  // fighting circular deps at 2am idgaf
  delete commandCompiler;
} 

void VirtualController::update() {
  if (inputHistory.size() > 0) {
    for (auto i : inputHistory.front()) {
      if (i.pressed)
       inputEventList.push_front(i); 
    }
  }
  inputHistory.push_front(InputFrameT());
}

void VirtualController::initCommandCompiler(){
  commandCompiler->init();
}

bool VirtualController::isPressed(Input input, bool strict) {
  if(input < 16 && strict){
    return (input == (currentState & 0x0F));
  } else {
    return (currentState & input);
  }
}

bool VirtualController::wasPressed(Input input, bool strict, int index, bool pressed) {
  int historySize = inputHistory.size();
  if (index >= historySize) {
    return false;
  }
  InputFrameT* eventList = &inputHistory[index];
  if (eventList->size() == 0) {
    return false;
  }

  for(InputEvent& event : *eventList) {
    if (event.valid) {
      if((pressed && event.pressed) || (!pressed && !event.pressed)){
        if (input <= 10 && strict) {
          return (input == (event.inputBit & 0x0F));       
        } else if(event.inputBit & input) {
          return true;
        }
      }
    }
  }
  // printf("wasnt pressed\n");
  return false;
}

bool VirtualController::wasPressedBuffer(Input input, bool strict, bool pressed) {
  int buffLen = 4;
  bool found = false;
  int historySize = inputHistory.size();

  for (int i = 0; i < buffLen && !found; ++i) {
    InputFrameT* eventList = &inputHistory[i];

    if (eventList->size() > 0) {
      for(InputEvent& event : *eventList) {
        if (event.valid) {
          if((pressed && event.pressed) || (!pressed && !event.pressed)){
            if (input <= 10 && strict) {
              found = (input == (event.inputBit & 0x0F));       
            } else if(event.inputBit & input) {
              // printf("checking non cardinal direction %d\n", input);
              // printf("was pressed\n");
              found = true;
            }
          }
        }
      }
    }
  }

  return found;
}

bool VirtualController::wasReleased(Input input, bool strict, int index) {
  return wasPressed(input, strict, index, false);
}

bool VirtualController::checkCommand(int commandIndex, bool faceRight) {
  // printf("in checkCommand\n");
  bool foundPart = false;
  bool foundCommand = false;
  bool breakFlag  = false;
  int searchOffset = 0;
  int firstFindOffet = 0;
  bool firstFind = true;

  if(commandIndex >= commandCompiler->commands.size()){
    return false;
  }

  bool clears = commandCompiler->commands[commandIndex].clears;
  std::vector<CommandNode>* command = &commandCompiler->commands[commandIndex].command;
  for (int i = command->size() - 1; i >= 0 && !breakFlag; --i) {
    CommandNode& funcNode = (*command)[i];

    for (int i = 0; (!foundPart) && (i < funcNode.bufferLength); ++i) {
      foundPart = (funcNode.function)(i+searchOffset, faceRight);
      if(foundPart){
        if (firstFind) {
          firstFindOffet = i;
          firstFind = false;
        }
        searchOffset += i;
      }
    }

    if(foundPart){
      foundPart = false;
      if(i == 0){
        foundCommand = true;
        if (clears) {
          for (int i = firstFindOffet; i < inputHistory.size(); ++i) {
             InputFrameT* eventList = &inputHistory[i];

            if (eventList->size() > 0) {
              for(InputEvent& event : *eventList) {
                event.valid = false;
              }
            }
          }
          // inputHistory.erase(inputHistory.begin() + firstFindOffet, inputHistory.end());
        }
      }
    } else {
      breakFlag = true;
    }
  }

  return foundCommand;
}

void VirtualController::setBit(Input bit) {
  // printf("setting bit %s\n", inputToString[bit]);
  currentState |= bit;
  inputHistory.front().push_back(InputEvent(bit, true));
  // printStickState();
}

void VirtualController::clearBit(Input bit) {
  currentState &= ~bit;
  inputHistory.front().push_back(InputEvent(bit, false));
}

void VirtualController::setBitOffset(uint16_t offset) {
  currentState |= (1 << offset);
}

void VirtualController::clearBitOffset(uint16_t offset) {
  currentState &= ~(1 << offset);
}

void VirtualController::setAxis(Input newState){
  uint8_t oldStickState = currentState & 0x0F;
  currentState &= ~0x0F;
  currentState |= (newState & 0x0F);

  uint8_t newStickState = currentState & 0x0F;
  InputFrameT& currentList = inputHistory.front();
  // printf("clearing old stick state setAxis: %s\n", inputToString[oldStickState]);
  currentList.push_back(InputEvent(oldStickState, false));
  currentList.push_back(InputEvent(newStickState, true));
}

void VirtualController::updateAxis(bool isXAxis) {
  int axis = isXAxis ? xAxis : yAxis;
  uint8_t offset = isXAxis ? 0 : 2;
  uint8_t oldStickState = currentState & 0x0F;
  InputFrameT& currentList = inputHistory.front();
  // printf("new axis value %d\n", axis);

  switch (axis) {
    case NEGATIVE:
      clearBitOffset(0+offset);
      setBitOffset(1+offset);
      // printf("negative\n");
      break;
    case NEUTRAL:
      clearBitOffset(0+offset);
      clearBitOffset(1+offset);
      // printf("neutral\n");
      break;
    case POSITIVE:
      clearBitOffset(1+offset);
      setBitOffset(0+offset);
      // printf("positive\n");
      break;
    default:
      printf("howd you get here??\n");
      break;
  }

  uint8_t newStickState = currentState & 0x0F;
  // printf("clearing old stick state updateAxis %s\n", inputToString[oldStickState]);
  currentList.push_back(InputEvent(oldStickState, false));
  currentList.push_back(InputEvent(newStickState, true));
}

void VirtualController::startCopyMode() {
  if (copyModeSlot == 1) {
    inputHistoryCopy.clear();
    inputStateCopy.clear();
  } else {
    inputHistoryCopyTwo.clear();
    inputStateCopyTwo.clear();
  }
  copyMode = true;
}

void VirtualController::stopCopyMode() {
  copyMode = false;
}

void VirtualController::playInputHistoryCopy(InputHistoryT* inputHistoryPointer) {
}

void VirtualController::printStickState(){
  std::bitset<16> stickState(currentState);
  std::cout << stickState << std::endl;
}

uint16_t VirtualController::getState() {
  return currentState;
}

void VirtualController::setState(uint16_t newState) {
  currentState = newState;
}

uint8_t VirtualController::getStickState() {
  return (currentState & 0x0F);
}

VirtualControllerObj VirtualController::saveState() {
  VirtualControllerObj controllerObj;
  controllerObj.currentState = currentState;
  controllerObj.prevState = prevState;
  for (int i = 0; (i < inputHistory.size()); ++i) {
    InputEvent input = inputHistory.at(i).back();
    controllerObj.inputHistory[i] = input;
  }
  return controllerObj;
}

void VirtualController::loadState(VirtualControllerObj stateObj) { 
  currentState = stateObj.currentState;
  prevState = stateObj.prevState;
  for (int i = 0; i < (inputHistory.size()); ++i) {
    InputEvent input = stateObj.inputHistory[i];
    printf("frame:%d input:%d pressed:%d\n", i, input.inputBit, input.pressed);

    InputFrameT currentFrame{input};
    inputHistory.at(i).clear();
    inputHistory.at(i) = currentFrame;
  }
  printf("virtualController loadHistory done\n");
}

void VirtualController::addNetInput(int input) {
  // prevState = currentState;
  // currentState = input;
  // inputHistory.front().emplace_back(InputEvent(currentState, true));
  // loop through bits in currentState
  // for each bit in current state,
  // if set..
  //  and not set before, send wasPressed event
  // if not set..
  //  and was set before, send wasReleased event
  // std::cout << "currentState:" << stickState << std::endl;
  // std::cout << "prevState:" << prevStickState << std::endl;
  // uint8_t oldStickState = prevState & 0x0F;
  // uint8_t newStickState = currentState & 0x0F;
  // if (oldStickState != newStickState) {
  //   // printf("clearing:%s setting:%s\n", inputToString[oldStickState], inputToString[newStickState]);
  //   inputHistory.front().emplace_back(InputEvent(oldStickState, false));
  //   inputHistory.front().emplace_back(InputEvent(newStickState, true));
  // }
  // std::bitset<16> stickState(currentState);
  // std::bitset<16> prevStickState(prevState);
  // for (int i = 0; i < 16; ++i) {
  //   uint16_t temp = 0;
  //   temp |= (1 << i);
  //   if (stickState.test(i) && !prevStickState.test(i)) {
  //     printf("detected button press for %s\n", inputToString[temp]);
  //     inputHistory.front().emplace_back(InputEvent(temp, true));
  //   } else if(!stickState.test(i) && prevStickState.test(i)){
  //     printf("detected button release for %s\n", inputToString[temp]);
  //     inputHistory.front().emplace_back(InputEvent(temp, false));
  //   }
  // }
}



void VirtualController::onNotify(const char* eventName) { }

