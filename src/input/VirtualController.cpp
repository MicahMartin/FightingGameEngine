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
  inputHistory.set_capacity(120);
}

VirtualController::~VirtualController() { } 

void VirtualController::setBit(uint16_t bit) {
  currentState |= bit;
  inputHistory.front().push_back(InputEvent(currentState, true));
  printStickState();
}

void VirtualController::clearBit(uint16_t bit) {
  currentState &= ~bit;
  inputHistory.front().push_back(InputEvent(currentState, false));
  printStickState();
}

void VirtualController::update() {
  inputHistory.push_front(std::vector<InputEvent>());
}

bool VirtualController::isPressed(Input input) {
  if(input == NOINPUT && ( currentState == 0 || ((currentState & LEFT) && (currentState & RIGHT)) )) {
    return true;
  }
  return (input == (currentState & 0x0F));
}

bool VirtualController::wasPressed(Input input, int index) {
  for(InputEvent event : inputHistory[index]) {
    if(input == NOINPUT ) {
      if(debugEnabled){
        printf("checking noInput, inputEvent bit is %d\n", event.inputBit);
      }
      if(event.inputBit == 0 || ((event.inputBit & LEFT) && (event.inputBit & RIGHT))){
        if(debugEnabled){
          printf("found noInput\n");
        }
        return true;
      }
    }
    if(event.pressed && event.inputBit & input){
      return true;
    }
  }
  return false;
}

bool VirtualController::wasReleased(Input input, int index) {
  for(InputEvent event : inputHistory[index]) {
    if(!event.pressed && event.inputBit & input){
      return true;
    }
  }
  return false;
}

bool VirtualController::checkCommand(int commandIndex, bool faceRight) {
  if(debugEnabled){
    printf("checking..\n");
  }

  // look for A
  std::vector<int> commandSequence = commandSequences[commandIndex];
  // commandSeq = {10, 6, 3, 2}

  bool found = false;
  bool breakFlag = false;
  bool foundSeq = false;
  int searchOffset = 0;

  for (int sCount = 0; sCount < commandSequence.size() && !breakFlag; ++sCount) {
    Input inputToCheck = inputMap[commandSequence[sCount]](faceRight); 
    if (debugEnabled) {
      printf("looking for %s at offset %d\n", inputToString[inputToCheck], searchOffset);
    }
    for (int i = 0; !found && i < 8; ++i) {
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

