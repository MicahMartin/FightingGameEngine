#include "inputManager.h"
#include <iostream>
#include <fstream>
#include "pugixml.hpp"

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {
  config.load_file("../data/p1Config.xml");
  for (pugi::xml_node item : config.first_child()) {
    std::cout << item.attribute("value").as_int() << std::endl;
    std::cout << item.text().as_int() << std::endl;
    bConf[item.attribute("value").as_int()] = item.text().as_int();
  }
}

uint32_t InputManager::getEventValue(SDL_Event event){
  // for each event, get the value for that event type
  // HashMap<int, int> userInputHashMap
  // uint16_t bitValForEvent = userInputHashMap[getInput(event)]
  switch (event.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
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

  while( SDL_PollEvent(&event) != 0 ){

    // printf("heres the current input value %s\n", std::bitset<32>(getEventValue(event)).to_string().c_str());
    // lets just say the user can only use one device and thats the end of that

    switch (event.type) {
      case SDL_KEYDOWN:
        std::cout << bConf[event.key.keysym.sym] << std::endl;
        //virtualController.setBits(bConf[event.key.keysym.sym]);
      break;

      case SDL_KEYUP:
        //virtualController.clearBits(bConf[event.key.keysym.sym]);
      break;

      //case SDL_JOYBUTTONDOWN:
      //break;

      //case SDL_JOYBUTTONUP:
      //break;

      //case SDL_JOYAXISMOTION:
      //break;

      case SDL_QUIT:
        notifyOne("game", "QUIT_REQUEST");
      break;
      default:
      break;
    }
  }

  // printf("heres the current byte of input %s\n", std::bitset<16>(inputEnum).to_string().c_str());
  virtualController.update();
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
