#include "inputManager.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {
}

uint16_t InputManager::getEventValue(SDL_Event event){
  // for each event, get the value for that event type
  // HashMap<int, int> userInputHashMap
  // uint16_t bitValForEvent = userInputHashMap[getInput(event)]
  switch (event.type) {
    case SDL_KEYDOWN:
      return event.key.keysym.sym;
    break;

    case SDL_JOYHATMOTION:
      return event.jhat.value;
      //if (event.jhat.value == SDL_HAT_CENTERED)
      //    SDL_Log(" centered");
      //if (event.jhat.value & SDL_HAT_UP)
      //    SDL_Log(" up");
      //if (event.jhat.value & SDL_HAT_RIGHT)
      //    SDL_Log(" right");
      //if (event.jhat.value & SDL_HAT_DOWN)
      //    SDL_Log(" down");
      //if (event.jhat.value & SDL_HAT_LEFT)
      //    SDL_Log(" left");
      //SDL_Log("\n");
    break;

    case SDL_JOYBUTTONDOWN:
      return event.jbutton.button;
      // SDL_Log("Joystick %d button %d down\n", event.jbutton.which, event.jbutton.button);
    break;

    default:
      return 0;
    break;
  }
}

void InputManager::update() {
  SDL_Event event;
  uint64_t forVirtualController = 0;

  while( SDL_PollEvent(&event) != 0 ){
    switch (event.type) {
      case SDL_QUIT:
        notifyOne("game", "QUIT_REQUEST");
      break;
      default:
        //Input i(event.type, getEventValue(event));
        //int inputFromConf = bConf[i];
        //forVirtualController |= inputFromConf;
      break;
    }
  }
  std::cout << "Heres the current byte of input" << std::bitset<64>(forVirtualController) << std::endl;

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
