#include "virtualController.h"
#include "inputManager.h"

#include <bitset>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>


VirtualController::VirtualController() {
}

VirtualController::~VirtualController() {
}

void VirtualController::update(uint16_t inputBits) {
  // printf("currentState %d \n", currentState);
  // std::cout << "Heres the current byte of input" << std::bitset<16>(inputByte) << std::endl;
  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  printf("frame %s\n", std::bitset<16>(inputBits).to_string().c_str());
  stickState = (inputBits & 0x0f);
  printf("stickState %s\n", std::bitset<4>(stickState).to_string().c_str());
  if (stickState == DOWNRIGHT) {
    printf("huzzah");
  }
  if(stickState == DOWN){
    printf("stick is in 2");
  }
}

void VirtualController::init() { 
}

//void VirtualController::setStickState(){
//}

//void VirtualController::setState(int stateVal) {
//}

//void VirtualController::setState(int stateVal, bool addToHistory) {
//  currentState = stateVal;
//  if(addToHistory){
//  }
//}

uint16_t VirtualController::getState() {

  return currentState;
}

void VirtualController::onNotify(const char* eventName){

}

int VirtualController::getInputHistorySize(){ return 0;};
