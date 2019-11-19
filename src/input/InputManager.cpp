#include "input/InputManager.h"
#include "input/VirtualController.h"
#include <iostream>
#include <fstream>

// input manager is gonna get all input events from SDL for all devices
// keep a map with input keycodes as the key and tuple of player number actual 'button code' as the value
// so lets say the map has {key: SDL_CODE, val: {playerNum:1, buttonVal: 0x1}} with the bits on the buttonVal byte corresponding to InputManager::Input
void InputManager::init() {
  // load the config(s)
  std::ifstream configFile("../data/buttonconf.json");
  configFile >> configJson;

   bConf.reserve(32);
   for(auto& item : configJson.items()){
     // TODO: Check for too much configItems
     // the 'value' is gonna be the button value for corresponding device
     // std::cout << item.attribute("value").as_int() << std::endl;
     // the node text is gonna be the bit for said button
     // std::cout << std::bitset<16>(item.text().as_int()) << std::endl;
     ConfItem myItem;
     myItem.user = item.value()["user"].get<uint8_t>();
     myItem.inputBit = (Input)std::stoi(item.value()["input"].get<std::string>(), 0 ,16);

     bConf[std::stoi(item.key())] = myItem;
   }
   configFile.close();
}

void InputManager::update() {
  SDL_Event event;

  for (VirtualController* controller : controllers) {
    controller->update();
  }

  while(SDL_PollEvent(&event) != 0){

    switch (event.type) {
      case SDL_KEYDOWN: {
        if(event.key.repeat == 0){
          if(bConf.find(event.key.keysym.sym) != bConf.end()){
            ConfItem* item = &bConf.at(event.key.keysym.sym);
            VirtualController* controller = controllers.at(item->user - 1);
            Input* inputBit = &item->inputBit;
            // is cardinal?
            if (*inputBit <= 8) {
              // printf("isCardinal!\n");
              bool isXAxis = *inputBit <= 2;
              if (isXAxis) {
                *inputBit == RIGHT ? controller->xAxis++ : controller->xAxis--;
              } else {
                *inputBit == UP ? controller->yAxis++ : controller->yAxis--;
              }
              // this calls setBit
              controller->updateAxis(isXAxis);
            } else {
              controller->setBit(*inputBit);
            }
          }
        }
        break;
      }

      case SDL_KEYUP: {
        if (bConf.find(event.key.keysym.sym) != bConf.end()) {
          ConfItem* item = &bConf.at(event.key.keysym.sym);
          VirtualController* controller = controllers.at(item->user - 1);
          Input* inputBit = &item->inputBit;
          // is cardinal?
          if (*inputBit <= 8) {
            // printf("isCardinal! clearing\n");
            bool isXAxis = *inputBit <= 2;
            if (isXAxis) {
              *inputBit == RIGHT ? controller->xAxis-- : controller->xAxis++;
            } else {
              *inputBit == UP ? controller->yAxis-- : controller->yAxis++;
            }
            // this calls clearBit
            controller->updateAxis(isXAxis);
          } else {
            controller->clearBit(*inputBit);
          }  
        }
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

VirtualController* InputManager::getVirtualController(int index){
  return controllers.at(index);
}

void InputManager::notifyOne(const char* observerName, const char* eventName){
  Observer* observer = observerList.at(observerName);
  observer->onNotify(eventName);
}
