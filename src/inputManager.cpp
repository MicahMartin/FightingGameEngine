#include "inputManager.h"
#include <iostream>
#include <fstream>
#include "pugixml.hpp"

InputManager::InputManager(){}

InputManager::~InputManager(){}

void InputManager::init() {
  config.load_file("../data/p1Config.xml");
  std::cout << "inital config" << std::endl;

  for (auto item : config.first_child()) {
    std::cout << item.attribute("value").as_int() << std::endl;
    std::cout << std::bitset<16>(item.text().as_int()) << std::endl;

    // set config to virtualConf
    if(item.attribute("value").as_int()){
      bConf[item.attribute("value").as_int()] = item.text().as_int();
    }
  }
}

void InputManager::update() {
  SDL_Event event;

  while( SDL_PollEvent(&event) != 0 ){

    // printf("heres the current input value %s\n", std::bitset<32>(getEventValue(event)).to_string().c_str());
    // lets just say the user can only use one device and thats the end of that

    switch (event.type) {
      case SDL_KEYDOWN:
        virtualController.setBits(bConf[event.key.keysym.sym]);
      break;

      case SDL_KEYUP:
        virtualController.clearBits(bConf[event.key.keysym.sym]);
      break;

      case SDL_JOYBUTTONDOWN:
        virtualController.setBits(bConf[event.jbutton.button]);
      break;

      case SDL_JOYBUTTONUP:
        virtualController.clearBits(bConf[event.jbutton.button]);
      break;

      // TODO
      //case SDL_JOYAXISMOTION:
      //break;
      //case SDL_JOYHATMOTION:
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
