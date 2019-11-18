#include "input/VirtualController.h"
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


VirtualController::VirtualController() { 
  newInputHistory.set_capacity(120);
}

VirtualController::~VirtualController() { } 

void VirtualController::setBit(uint16_t bit) {
  currentState |= bit;
  newInputHistory.front().push_back(InputEvent(bit, true));
}

void VirtualController::clearBit(uint16_t bit) {
  currentState &= ~bit;
  newInputHistory.front().push_back(InputEvent(bit, false));
}

void VirtualController::setBitOffset(uint16_t offset) {
  currentState |= (1 << offset);
}

void VirtualController::clearBitOffset(uint16_t offset) {
  currentState &= ~(1 << offset);
}

void VirtualController::updateAxis(bool isXAxis) {
  int axis = isXAxis ? xAxis : yAxis;
  uint8_t offset = isXAxis ? 0 : 2;
  uint8_t oldStickState = currentState & 0x0F;
  std::list<InputEvent>& currentList = newInputHistory.front();
  printf("new axis value %d\n", axis);

  switch (axis) {
    case NEGATIVE:
      clearBitOffset(0+offset);
      setBitOffset(1+offset);
      printf("negative\n");
      break;
    case NEUTRAL:
      clearBitOffset(0+offset);
      clearBitOffset(1+offset);
      printf("neutral\n");
      break;
    case POSITIVE:
      clearBitOffset(1+offset);
      setBitOffset(0+offset);
      printf("positive\n");
      break;
    default:
      printf("howd you get here??\n");
      break;
  }
  printStickState();

  uint8_t newStickState = currentState & 0x0F;
  currentList.push_back(InputEvent(oldStickState, false));
  currentList.push_back(InputEvent(newStickState, true));
}

void VirtualController::update() {
  newInputHistory.push_front(std::list<InputEvent>());
}

bool VirtualController::isPressed(Input input) {
  return (input == (currentState & 0x0F));
}

bool VirtualController::wasPressed(Input input, int index) {
  int historySize = newInputHistory.size();
  // printf("the inputHistory size %d, the index %d\n", historySize, index);
  if (index >= historySize) {
    printf("returning false\n");
    return false;
  }
  // printf("checking eventList\n");
  std::list<InputEvent>* eventList = &newInputHistory[index];
  // printf("eventListSize %ld\n", eventList->size());
  if (eventList->size() == 0) {
    // printf("empty eventList\n");
    // printStickState();
    return false;
  }

  for(InputEvent event : *eventList) {
    if(event.pressed){
      if (input <= 10) {
        // printf("checking cardinal direction %s\n", inputToString[input]);
        return (event.inputBit == input);       
      } else if(event.inputBit & input) {
        // printf("checking non cardinal direction %d\n", input);
        return true;
      }
    }
  }
  return false;
}

bool VirtualController::wasReleased(Input input, int index) {
  int historySize = newInputHistory.size();
  if (index >= historySize) {
    return false;
  }
  for(InputEvent event : newInputHistory[historySize - index+1]) {
    if(!event.pressed){
      if (input <= 10) {
        return (event.inputBit == input);       
      } else if(event.inputBit & input) {
        return true;
      }
    }
  }
  return false;
}

bool VirtualController::checkCommand(int commandIndex, bool faceRight) {
  bool found = false;
  bool breakFlag = false;
  bool foundSeq = false;
  int searchOffset = 0;
  int bufferLen = 8;
  std::vector<int> commandSequence = commandSequences[commandIndex];


  for (int sCount = 0; sCount < commandSequence.size() && !breakFlag; ++sCount) {
    Input inputToCheck = inputMap[commandSequence[sCount]](faceRight); 
    if (debugEnabled) {
      printf("looking for %s at offset %d\n", inputToString[inputToCheck], searchOffset);
    }
    for (int i = 0; !found && i < bufferLen; ++i) {
      found = wasPressed(inputToCheck, i+searchOffset);
      if(found){
        searchOffset += i;
        if (debugEnabled) {
          printf("found %s at index %d\n", inputToString[inputToCheck], i);
        }
      }
    }
    if(found){
      found = false;
      if(sCount+1 == commandSequence.size()){
        foundSeq = true;
      }
    } else {
      breakFlag = true;
    }
  }
  if (foundSeq) {
    if (debugEnabled) {
      printf("found sequence for command sequence %d\n", commandIndex);
    }
  }
  return foundSeq;
}

void VirtualController::printStickState(){
  std::bitset<4> stickState(currentState);
  std::cout << stickState << std::endl;
}

uint16_t VirtualController::getState() {
  return currentState;
}

uint8_t VirtualController::getStickState() {
  return (currentState& 0x0f);
}



void VirtualController::onNotify(const char* eventName) { }

