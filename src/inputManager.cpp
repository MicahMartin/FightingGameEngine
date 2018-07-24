#include "inputManager.h"
#include <bitset>
#include <iostream>

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {}

void InputManager::update() {

  SDL_Event event;

  while( SDL_PollEvent(&event) != 0 ){
    switch (event.type) {
      case SDL_KEYDOWN:
        setBit(event);
      break;
      case SDL_KEYUP:
        clearBit(event);
      break;
      case SDL_QUIT:
        notifyOne("game", "QUIT_REQUEST");
      break;
      default:
      break;
    }
  }
  // std::cout << "Heres the current byte of input" << std::bitset<16>(inputEnum) << std::endl;
  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  virtualController.update(inputEnum);
}

void InputManager::setBit(SDL_Event event) {

  switch (event.key.keysym.sym) {
    // Directional inputs
    // TODO: allow user to create config like {'buttonForDown': 'userDefinedButtonForDown'} 
    case SDLK_DOWN:
      inputEnum |= DOWN;
    break;

    case SDLK_RIGHT:
      inputEnum |= RIGHT;
    break;

    case SDLK_LEFT:
      inputEnum |= LEFT;
    break;

    case SDLK_UP:
      inputEnum |= UP;
    break;
  }
}

void InputManager::clearBit(SDL_Event event) {

  switch (event.key.keysym.sym) {
    // Directional inputs
    case SDLK_DOWN:
      inputEnum &= ~DOWN;
    break;

    case SDLK_RIGHT:
      inputEnum &= ~RIGHT;
    break;

    case SDLK_LEFT:
      inputEnum &= ~LEFT;
    break;

    case SDLK_UP:
      inputEnum &= ~UP;
    break;
  }
}


uint16_t InputManager::getInputByte(){

  // get the most recent input for this game tick
  return inputEnum;
  // copy everything else into input history list
}

int InputManager::getInputHistorySize(){ return 0;};

VirtualController* InputManager::getVirtualController(){
  return &virtualController;
}

void InputManager::addObserver(const char* observerName, Observer* observer){
  printf("Observer added to inputManager \n");
  observerList.insert(std::make_pair(observerName, observer));
};

void InputManager::removeObserver(const char* observerName){

  observerList.erase(observerName);
};

void InputManager::notifyAll(const char* eventName){

  for( auto const& [key, observer] : observerList ){
    observer->onNotify(eventName);
  }
};

void InputManager::notifyOne(const char* observerName, const char* eventName){
  Observer* observer = observerList.at(observerName);
  observer->onNotify(eventName);

}
