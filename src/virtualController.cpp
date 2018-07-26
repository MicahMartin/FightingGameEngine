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

void VirtualController::update() {
  // printf("currentState %d \n", currentState);
  // std::cout << "Heres the current byte of input" << std::bitset<16>(inputByte) << std::endl;
  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  setStickState();
}

void VirtualController::init() { 
  std::ifstream userConfig("../data/userConf/p1ControllerConfig.json", std::ifstream::binary);
  userConfig >> buttonConfig;
  printf("The user config %s\n", buttonConfig.dump().c_str());
  // iterate through the conf object, convert all user config key values to their corresponding hex codes
  // only need to do this once every time the file is changed
  // for (auto jsonObj : buttonConfig) {
  //  const char* fuck = jsonObj.asCString();
  // }
}

void VirtualController::setBit(SDL_Event event) {
  switch (event.key.keysym.sym) {
    // Directional inputs
    // TODO: allow user to create config like {'buttonForDown': sdlGetKeyCode(userDefinedButtonForDown)} 
    case SDLK_DOWN:
      inputByte |= DOWN;
    break;

    case SDLK_f:
      inputByte |= RIGHT;
    break;

    case SDLK_s:
      inputByte |= LEFT;
    break;

    case SDLK_SPACE:
      inputByte |= UP;
    break;
  }
}

void VirtualController::clearBit(SDL_Event event) {

  switch (event.key.keysym.sym) {
    // Directional inputs
    case SDLK_d:
      inputByte &= ~DOWN;
    break;

    case SDLK_f:
      inputByte &= ~RIGHT;
    break;

    case SDLK_s:
      inputByte &= ~LEFT;
    break;

    case SDLK_SPACE:
      inputByte &= ~UP;
    break;
  }
}

void VirtualController::setStickState(){
  switch (inputByte) {
    case (NEUTRAL):
      setState(N);
      break;
    case (DOWN):
      setState(D);
      break;
    case (RIGHT):
      setState(R);
      break;
    case (LEFT):
      setState(L);
      break;
    case (UP):
      setState(U);
      break;
    case (DOWN | RIGHT):
      setState(DR);
      break;
    case (DOWN |  LEFT):
      setState(DL);
      break;
    case (DOWN |  UP):
      // weirdo byte
      setState(U, false);
      break;
    case (RIGHT |  LEFT):
      // weirdo byte
      setState(N, false);
      break;
    case (UP |  RIGHT):
      setState(UR);
      break;
    case (UP |  LEFT):
      setState(UL);
      break;

    // everything past this point is a weirdo byte
    case (DOWN |  LEFT | RIGHT):
      setState(D, false);
      break;
    case (UP |  LEFT | RIGHT):
      setState(U, false);
      break;
    case (DOWN |  UP | RIGHT):
      setState(UR, false);
      break;
    case (DOWN |  UP | LEFT):
      setState(UL, false);
      break;
    case (DOWN |  LEFT | RIGHT | UP):
      setState(U, false);
      break;
    default:
      break;
  }
}

void VirtualController::setState(int stateVal) {
  setState(stateVal, true);
}

void VirtualController::setState(int stateVal, bool addToHistory) {
  currentState = stateVal;
  if(addToHistory){
    // add to input history
    // dont add to input history if we got to the state with a weird input 
    // ( like left + right should take you to neutral, but shouldn't count as a neutral input when checking for sequences )
    // this way people cant do shit like, ( 6 -> 4+6 -> 6 ) and get a dash
    // damn keyboard and hitbox players
  }
}

int VirtualController::getState() {

  return currentState;
}

void VirtualController::onNotify(const char* eventName){

}

uint16_t VirtualController::getInputByte(){

  // get the most recent input for this game tick
  return inputByte;
}

int VirtualController::getInputHistorySize(){ return 0;};
