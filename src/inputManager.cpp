#include "inputManager.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {
}

uint32_t InputManager::getEventKey(uint16_t sdlEventType, uint16_t sdlEventValue){
}

uint16_t InputManager::getEventValue(SDL_Event event){
  // for each event, get the value for that event type
  // HashMap<int, int> userInputHashMap
  // uint16_t bitValForEvent = userInputHashMap[getInput(event)]
}

void InputManager::update() {
  SDL_Event event;
  
  uint32_t forVirtualController;
  while( SDL_PollEvent(&event) != 0 ){
    std::cout << "Heres the current byte of input" << std::bitset<32>(event.type) << std::endl;
  }
  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  // virtualController.update();
}

VirtualController* InputManager::getVirtualController(){
  return &virtualController;
}

// Subject 
void InputManager::addObserver(const char* observerName, Observer* observer){
  printf("Observer added to inputManager \n");
  observerList.insert(std::make_pair(observerName, observer));
};

void InputManager::removeObserver(const char* observerName){

  observerList.erase(observerName);
};

void InputManager::notifyAll(const char* eventName){

  for( const auto& [key, observer] : observerList ){
    observer->onNotify(eventName);
  }
};

void InputManager::notifyOne(const char* observerName, const char* eventName){
  Observer* observer = observerList.at(observerName);
  observer->onNotify(eventName);

}
