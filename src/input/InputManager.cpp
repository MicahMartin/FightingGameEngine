#include "input/InputManager.h"
#include "input/VirtualController.h"
#include <iostream>
#include <fstream>
#include <sstream>

// input manager is gonna get all input events from SDL for all devices
// keep a map with input keycodes as the key and tuple of player number actual 'button code' as the value
// so lets say the map has {key: SDL_CODE, val: {playerNum:1, buttonVal: 0x1}} with the bits on the buttonVal byte corresponding to InputManager::Input
void InputManager::init() {
  // load the config(s)
  std::ifstream configFile("../data/buttonconf.json");
  nlohmann::json newJson;
  configFile >> newJson;

   bConf.reserve(32);
   bConf.clear();
   for(auto& item : newJson.items()){
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
   configJson = newJson;
}

void InputManager::update() {
  SDL_Event event;

  for (VirtualController* controller : controllers) {
    controller->update();
  }

  while(SDL_PollEvent(&event) != 0){

    if (!keySelectionMode) {
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
            } else { }
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
    } else {
      switch (event.type) {
        case SDL_KEYDOWN: {
          if(event.key.repeat == 0){
            printf("the sdl keycode for the thing just pressed: %d\n", event.key.keysym.sym);
          }
          break;
        }

        case SDL_KEYUP: {
          printf("configuring item:%d, the sdl keycode for the thing just released : %d\n", configCounter, event.key.keysym.sym);
          configArray[configCounter] = event.key.keysym.sym;
          configCounter++;
          if (configCounter == 8) {
            configCounter = 0;
            keySelectionMode = false;
            writeConfig(configArray);
          }
          break;
        }

        case SDL_JOYBUTTONDOWN: {
        }
        
        case SDL_JOYBUTTONUP:
          // clearBit(bConf[event.jbutton.button]);
        break;
        case SDL_QUIT:
          keySelectionMode = false;
        break;
        default:
        break;
      }

    }
  }
}

void InputManager::writeConfig(int* configArray){
  int itemCounter = 0;
  std::vector<std::string> removalKeys;
  for (auto i : configJson.items()) {
    int user = i.value().at("user");
    printf("the key:%s the user for this item: %d\n", i.key().c_str(), user);
    if(user == userBeingConfig){
      removalKeys.push_back(i.key());
    }
  }
  
  for (auto i : removalKeys) {
    configJson.erase(i);
    printf("removalKey:%s \n", i.c_str());
  }
  printf("configJsonNewSize:%d\n", configJson.size());

  for(auto input : inputTemplate){
    nlohmann::json obj = nlohmann::json::object();
    obj["user"] = userBeingConfig;
    std::stringstream stream;
    stream << std::hex << input;
    std::string result( stream.str() );

    obj["input"] = "0x" + result;
    printf("the input:%x\n", input);
    configJson[std::to_string(configArray[itemCounter])] = obj;

    itemCounter++;
    if(itemCounter == 8){
      break;
    }
  }

  std::ofstream newConfigFile("../data/buttonconf.json");
  configJson >> newConfigFile;
  newConfigFile.close();
  init();
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
