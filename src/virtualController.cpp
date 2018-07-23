#include "virtualController.h"
#include "inputManager.h"

VirtualController::VirtualController() {
}

VirtualController::~VirtualController() {
}

void VirtualController::update(uint16_t inputByte) {
  printf(" input byte val %d\n", inputByte);
  switch (inputByte) {
    case (InputManager::NEUTRAL):
      setState(NEUTRAL);
      break;
    case (InputManager::DOWN):
      setState(DOWN);
      break;
    case (InputManager::RIGHT):
      setState(FORWARD);
      break;
    case (InputManager::LEFT):
      setState(BACK);
      break;
    case (InputManager::UP):
      setState(UP);
      break;
    case (InputManager::DOWN | InputManager::RIGHT):
      setState(DOWNFORWARD);
      break;
    case (InputManager::DOWN |  InputManager::LEFT):
      setState(DOWNBACK);
      break;
    case (InputManager::DOWN |  InputManager::UP):
      // weirdo byte
      setState(UP, false);
      break;
    case (InputManager::RIGHT |  InputManager::LEFT):
      // weirdo byte
      setState(NEUTRAL, false);
      break;
    case (InputManager::UP |  InputManager::RIGHT):
      setState(UPFORWARD);
      break;
    case (InputManager::UP |  InputManager::LEFT):
      setState(UPBACK);
      break;

    // everything past this point is a weirdo byte
    case (InputManager::DOWN |  InputManager::LEFT | InputManager::RIGHT):
      setState(DOWN, false);
      break;
    case (InputManager::UP |  InputManager::LEFT | InputManager::RIGHT):
      setState(UP, false);
      break;
    case (InputManager::DOWN |  InputManager::UP | InputManager::RIGHT):
      setState(UPFORWARD, false);
      break;
    case (InputManager::DOWN |  InputManager::UP | InputManager::LEFT):
      setState(UPBACK, false);
      break;
    case (InputManager::DOWN |  InputManager::LEFT | InputManager::RIGHT | InputManager::UP):
      setState(UP, false);
      break;
    default:
      break;
      
  }
  printf("currentState %d \n", currentState);
}

void VirtualController::setState(int stickState) {
  setState(stickState, true);
}
void VirtualController::setState(int stickState, bool addToHistory) {
  currentState = stickState;
  if(addToHistory){
    // add to input history
    // dont add to input history if we got to the state with a weird input 
    // ( like left + right should take you to neutral, but shouldn't count as a neutral input when checking for sequences )
    // this way people cant do shit like, ( 6 -> 4+6 -> 6 ) and get a dash
    // damn keyboard and hitbox players
  }
}

void VirtualController::onNotify(const char* eventName){

}
