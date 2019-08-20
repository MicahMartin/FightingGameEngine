#include "input/InputManager.h"
#include "input/VirtualController.h"
#include <iostream>
#include <fstream>

// input manager is gonna get all input events from SDL for all devices
// keep a map with input keycodes as the key and tuple of player number actual 'button code' as the value
// so lets say the map has {key: SDL_CODE, val: {playerNum:1, buttonVal: 0x1}} with the bits on the buttonVal byte corresponding to InputManager::Input

InputManager::InputManager(){ }

InputManager::~InputManager(){ }

void InputManager::init() {
  // load the config(s)
  std::ifstream configFile("../data/buttonconf.json");
  configFile >> configJson;

   for(auto& item : configJson.items()){
     // the 'value' is gonna be the button value for corresponding device
     // std::cout << item.attribute("value").as_int() << std::endl;
     // the node text is gonna be the bit for said button
     // std::cout << std::bitset<16>(item.text().as_int()) << std::endl;
     ConfItem myItem;
     myItem.user = item.value()["user"].get<int>();
     myItem.inputBit = std::stoi(item.value()["input"].get<std::string>(), 0 ,16);
     myItem.inputString = item.value()["inputString"].get<std::string>();

     bConf[std::stoi(item.key())] = myItem;
     // set config to virtualConf
     // if(item.attribute("value").as_int()){
     //   bConf[item.attribute("value").as_int()] = item.text().as_int();
     // }
   }
}

void InputManager::update() {
  SDL_Event event;

  for (VirtualController* controller : controllers) {
    controller->update();
  }

  while(SDL_PollEvent(&event) != 0){

    switch (event.type) {
      case SDL_KEYDOWN: {
        try {
          ConfItem item = bConf.at(event.key.keysym.sym);
          controllers.at(item.user - 1)->setBit(item.inputBit);
        } catch(std::exception e) { }
        break;
      }

      case SDL_KEYUP: {
        try {
          ConfItem item = bConf.at(event.key.keysym.sym);
          controllers.at(item.user - 1)->clearBit(item.inputBit);
        } catch(std::exception e) { }
        break;
      }

      case SDL_JOYBUTTONDOWN: {

        // setBit(bConf[event.jbutton.button]);
        break;
      }
      
      case SDL_JOYBUTTONUP:
        // clearBit(bConf[event.jbutton.button]);
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

void InputManager::addVirtualController(VirtualController* controller){
  controllers.push_back(controller);
}

void InputManager::notifyOne(const char* observerName, const char* eventName){
  Observer* observer = observerList.at(observerName);
  observer->onNotify(eventName);
}
