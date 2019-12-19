#include "input/VirtualController.h"
#include "input/CommandCompiler.h"
#include <bitset>
#include <iostream>
#include <nlohmann/json.hpp>

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

std::map<Input, const char*> VirtualController::inputToString = {
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

VirtualController::VirtualController() { 
  inputHistory.set_capacity(120);
  inputEventList.set_capacity(10);
  commandCompiler = new CommandCompiler();
  commandCompiler->controllerPointer = this;
}

VirtualController::~VirtualController() { 
  // fighting circular deps at 2am idgaf
  delete commandCompiler;
} 

void VirtualController::update() {
  inputHistory.push_front(std::list<InputEvent>());
}

void VirtualController::initCommandCompiler(){
  commandCompiler->init();
}

bool VirtualController::isPressed(Input input, bool strict) {
  if(input <= 10 && strict){
    return (input == (currentState & 0x0F));
  } else {
    return input & currentState;
  }
}

bool VirtualController::wasPressed(Input input, bool strict, int index, bool pressed) {
  // printf("in the real wasPressed\n");
  int historySize = inputHistory.size();
  // printf("the inputHistory size %d, the index %d\n", historySize, index);
  if (index >= historySize) {
    return false;
  }
  // printf("checking eventList\n");
  std::list<InputEvent>* eventList = &inputHistory[index];
  // printf("eventListSize %ld\n", eventList->size());
  if (eventList->size() == 0) {
    // printStickState();
    return false;
  }

  for(InputEvent event : *eventList) {
    if((pressed && event.pressed) || (!pressed && !event.pressed)){
      if (input <= 10 && strict) {
        // printf("checking cardinal direction %s\n", inputToString[input]);
        return (input == (event.inputBit & 0x0F));       
      } else if(event.inputBit & input) {
        // printf("checking non cardinal direction %d\n", input);
        // printf("was pressed\n");
        return true;
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
  if (buffLen >= historySize) {
    return false;
  }

  for (int i = 0; i < buffLen && !found; ++i) {
    std::list<InputEvent>* eventList = &inputHistory[i];

    if (eventList->size() > 0) {
      for(InputEvent& event : *eventList) {
        if((pressed && event.pressed) || (!pressed && !event.pressed)){
          if (input <= 10 && strict) {
            // printf("checking cardinal direction %s\n", inputToString[input]);
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
  int bufferLen = 8;

  if(commandIndex >= commandCompiler->commandFunctionList.size()){
    return false;
  }

  std::vector<CommandFunction>* commandFuncs = &commandCompiler->commandFunctionList[commandIndex];
  for (int i = commandFuncs->size() - 1; i >= 0 && !breakFlag; --i) {
    auto func = (*commandFuncs)[i];

    for (int i = 0; (!foundPart) && (i < bufferLen); ++i) {
      foundPart = (func)(i+searchOffset, faceRight);
      if(foundPart){
        searchOffset += i;
      }
    }

    if(foundPart){
      foundPart = false;
      if(i == 0){
        foundCommand = true;
      }
    } else {
      breakFlag = true;
    }
  }

  return foundCommand;
}

void VirtualController::setBit(uint16_t bit) {
  currentState |= bit;
  inputHistory.front().push_back(InputEvent(currentState, true));
  inputEventList.push_front(InputEvent(bit, true));
}

void VirtualController::clearBit(uint16_t bit) {
  currentState &= ~bit;
  inputHistory.front().push_back(InputEvent(bit, false));
  // inputEventList.push_front(InputEvent(bit, false));
}

void VirtualController::setBitOffset(uint16_t offset) {
  currentState |= (1 << offset);
}

void VirtualController::clearBitOffset(uint16_t offset) {
  currentState &= ~(1 << offset);
}

void VirtualController::setAxis(Input newState){
  uint8_t oldStickState = currentState & 0x0F;
  currentState &= 0xF0;
  currentState |= (newState & 0x0F);

  uint8_t newStickState = currentState & 0x0F;
  std::list<InputEvent>& currentList = inputHistory.front();

  currentList.push_back(InputEvent(oldStickState, false));
  currentList.push_back(InputEvent(newStickState, true));
  if(newStickState != NOINPUT){
    inputEventList.push_front(InputEvent(newStickState, true));
  }
}

void VirtualController::updateAxis(bool isXAxis) {
  int axis = isXAxis ? xAxis : yAxis;
  uint8_t offset = isXAxis ? 0 : 2;
  uint8_t oldStickState = currentState & 0x0F;
  std::list<InputEvent>& currentList = inputHistory.front();
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
  printStickState();

  uint8_t newStickState = currentState & 0x0F;
  currentList.push_back(InputEvent(oldStickState, false));
  currentList.push_back(InputEvent(newStickState, true));
  if(newStickState != NOINPUT){
    inputEventList.push_front(InputEvent(newStickState, true));
  }
}

void VirtualController::startCopyMode() {
  inputHistoryCopy.clear();
  copyMode = true;
}

void VirtualController::stopCopyMode() {
  copyMode = false;
}

void VirtualController::playInputHistoryCopy(InputHistoryT inputHistoryPointer) {

}

void VirtualController::printStickState(){
  std::bitset<4> stickState(currentState);
  std::cout << stickState << std::endl;
}

uint16_t VirtualController::getState() {
  return currentState;
}

uint8_t VirtualController::getStickState() {
  return (currentState & 0x0F);
}



void VirtualController::onNotify(const char* eventName) { }

