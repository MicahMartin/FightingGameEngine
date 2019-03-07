#include "input/InputManager.h"
#include <iostream>
#include <fstream>
#include "pugixml.hpp"

InputManager::InputManager(){ }

InputManager::~InputManager(){ }

void InputManager::init() {
  // load the config(s)
  config.load_file("../data/p1Config.xml");

  for (auto item : config.first_child()) {
    // the 'value' is gonna be the button value for corresponding device
    std::cout << item.attribute("value").as_int() << std::endl;
    // the node text is gonna be the bit for said button
    std::cout << std::bitset<16>(item.text().as_int()) << std::endl;

    // set config to virtualConf
    if(item.attribute("value").as_int()){
      bConf[item.attribute("value").as_int()] = item.text().as_int();
    }
  }
}

void InputManager::update() {
  SDL_Event event;
  virtualController.clearPressed();
  virtualController.clearReleased();

  while( SDL_PollEvent(&event) != 0 ){

    switch (event.type) {
      case SDL_KEYDOWN:
        virtualController.setBit(bConf[event.key.keysym.sym]);
        virtualController.isPressed(bConf[event.key.keysym.sym]);
      break;

      case SDL_KEYUP:
        virtualController.clearBit(bConf[event.key.keysym.sym]);
        virtualController.isReleased(bConf[event.key.keysym.sym]);
      break;

      case SDL_JOYBUTTONDOWN:
        virtualController.setBit(bConf[event.jbutton.button]);
        virtualController.isPressed(bConf[event.key.keysym.sym]);
      break;

      case SDL_JOYBUTTONUP:
        virtualController.clearBit(bConf[event.jbutton.button]);
        virtualController.isReleased(bConf[event.key.keysym.sym]);
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
