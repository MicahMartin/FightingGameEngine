#include "input/InputManager.h"
#include "input/VirtualController.h"
#include <iostream>
#include <fstream>
#include <sstream>

// input manager is gonna get all input events from SDL for all devices
// keep a map with input keycodes as the key and tuple of player number actual 'button code' as the value
// so lets say the map has {key: SDL_CODE, val: {playerNum:1, buttonVal: 0x1}} with the bits on the buttonVal byte corresponding to InputManager::Input
void InputManager::init() {
  if( SDL_NumJoysticks() < 1 ) {
    printf( "Warning: No joysticks connected!\n" );
  }
  else {
    //Load joystick
    p1SDLController = SDL_JoystickOpen( 0 );
    p2SDLController = SDL_JoystickOpen( 1 );
    if( p1SDLController == NULL ) {
      printf( "Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError() );
    } else {
      printf("SDL Controller 1 initialized\n");
    }
    if( p2SDLController == NULL ) {
      printf( "Warning: Unable to open game controller 2! SDL Error: %s\n", SDL_GetError() );
    } else {
      printf("SDL Controller 2 initialized\n");
    }
  }
  initConfig("../data/buttonconf.json", &bConf, &configJson);
  initConfig("../data/p1buttonconf.json", &p1bConf, &p1configJson);
  initConfig("../data/p2buttonconf.json", &p2bConf, &p2configJson);
}

void InputManager::initConfig(const char* fileName, ConfT* config, nlohmann::json* confJson) {
  // load the config(s)
  std::ifstream configFile(fileName);
  nlohmann::json newJson;
  configFile >> newJson;

  config->reserve(32);
  config->clear();
  for(auto& item : newJson.items()){
    // TODO: Check for too much configItems
    // the 'value' is gonna be the button value for corresponding device
    // std::cout << item.attribute("value").as_int() << std::endl;
    // the node text is gonna be the bit for said button
    // std::cout << std::bitset<16>(item.text().as_int()) << std::endl;
    ConfItem myItem;
    myItem.user = item.value()["user"].get<uint8_t>();
    myItem.inputBit = (Input)std::stoi(item.value()["input"].get<std::string>(), 0 ,16);
    (*config)[std::stoi(item.key())] = myItem;
  }
  configFile.close();
  *confJson = newJson;
}

void InputManager::update() {
  SDL_Event event;

  for (VirtualController* controller : controllers) {
    if (controller->playbackMode) {
      std::list<InputEvent> currentFrame = controllers[0]->inputHistoryCopy[controller->playbackCounter];
      controller->inputHistory.push_front(currentFrame);
      controller->currentState = controllers[0]->inputStateCopy[controller->playbackCounter];
      controller->playbackCounter++;
    } else { 
      controller->update();
    }
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
              if (!controller->playbackMode) {
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
            } else { 
              // item not found
            }
          }
          break;
        }

        case SDL_KEYUP: {
          if (event.key.keysym.sym == SDLK_5) {
            controllers[0]->copyMode ? controllers[0]->stopCopyMode() : controllers[0]->startCopyMode();
            printf("copyMode toggled to %d, copySize:%ld\n", controllers[0]->copyMode, controllers[0]->inputHistoryCopy.size());
          };
          if (event.key.keysym.sym == SDLK_6) {
            printf("setting input history to the copy %ld\n", controllers[0]->inputHistoryCopy.size());
            controllers[1]->playbackMode = true;
          };
          if (bConf.find(event.key.keysym.sym) != bConf.end()) {
            ConfItem* item = &bConf.at(event.key.keysym.sym);
            VirtualController* controller = controllers.at(item->user - 1);
            if (!controller->playbackMode) {
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
          }
          break;
        }

        case SDL_JOYBUTTONDOWN: {
          SDL_JoyHatEvent* jhatEvent = &event.jhat;
          SDL_Joystick* stick = SDL_JoystickFromInstanceID(jhatEvent->which);
          VirtualController* controller = stickToVC[stick];
          if (!controller->playbackMode) {
            ConfT* conf;
            if(controller->controllerIndex == 1){
              conf = &p1bConf;
            } else if(controller->controllerIndex == 2){
              conf = &p2bConf;
            }

            if(conf != NULL && conf->count(event.jbutton.button)){
              ConfItem* item = &conf->at(event.jbutton.button);
              Input* inputBit = &item->inputBit;
              printf("found item from jbutton %d with val: %d\n", event.jbutton.button, *inputBit);
              controller->setBit(*inputBit);
            }
          }
          break;
        }
        
        case SDL_JOYBUTTONUP: {
          SDL_JoyHatEvent* jhatEvent = &event.jhat;
          SDL_Joystick* stick = SDL_JoystickFromInstanceID(jhatEvent->which);
          VirtualController* controller = stickToVC[stick];
          if (!controller->playbackMode) {
            ConfT* conf;
            if(controller->controllerIndex == 1){
              conf = &p1bConf;
            } else if(controller->controllerIndex == 2){
              conf = &p2bConf;
            }

            if(conf != NULL && conf->count(event.jbutton.button)){
              ConfItem* item = &conf->at(event.jbutton.button);
              Input* inputBit = &item->inputBit;
              printf("found item from jbutton %d with val: %d\n", event.jbutton.button, *inputBit);
              controller->clearBit(*inputBit);
            }
          }
          break;
        }
        case SDL_JOYHATMOTION: {
          SDL_JoyHatEvent* jhatEvent = &event.jhat;
          SDL_Joystick* stick = SDL_JoystickFromInstanceID(jhatEvent->which);
          VirtualController* controller = stickToVC[stick];
          if (controller != NULL && !controller->playbackMode) {
            switch (jhatEvent->value) {
              case SDL_HAT_CENTERED:
                controller->setAxis(NOINPUT);
                printf("setting axis to noinput\n");
                controller->printStickState();
                break;
              case SDL_HAT_RIGHT:
                controller->setAxis(RIGHT);
                printf("setting axis to right\n");
                controller->printStickState();
                break;
              case SDL_HAT_LEFT:
                controller->setAxis(LEFT);
                printf("setting axis to left\n");
                controller->printStickState();
                break;
              case SDL_HAT_UP:
                controller->setAxis(UP);
                printf("setting axis to up\n");
                controller->printStickState();
                break;
              case SDL_HAT_DOWN:
                controller->setAxis(DOWN);
                printf("setting axis to down\n");
                controller->printStickState();
                break;
              case SDL_HAT_RIGHTDOWN:
                controller->setAxis(DOWNRIGHT);
                printf("setting axis to downright\n");
                controller->printStickState();
                break;
              case SDL_HAT_RIGHTUP:
                controller->setAxis(UPRIGHT);
                printf("setting axis to rightup\n");
                controller->printStickState();
                break;
              case SDL_HAT_LEFTDOWN:
                controller->setAxis(DOWNLEFT);
                printf("setting axis to leftdown\n");
                controller->printStickState();
                break;
              case SDL_HAT_LEFTUP:
                controller->setAxis(UPLEFT);
                printf("setting axis to leftup\n");
                controller->printStickState();
                break;
              default:
                break;
            }
          }
        break;
        }
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
          if (bConf.find(event.key.keysym.sym) != bConf.end()) {
            ConfItem* item = &bConf.at(event.key.keysym.sym);
            Input* inputBit = &item->inputBit;
            if (*inputBit == MK) {
              keySelectionMode = false;
            }
          }
          break;
        }

        case SDL_JOYBUTTONDOWN: {
          break;
        }
        
        case SDL_JOYBUTTONUP: {
          printf("configuring item:%d, the button keycode for the thing just released : %d\n", configCounter, event.jbutton.button);
          buttonConfigArray[configCounter] = event.jbutton.button;
          configCounter++;
          if (configCounter == 4) {
            configCounter = 0;
            printf("writing button config\n");
            keySelectionMode = false;
            writeButtonConfig();
          }
          break;
        }
        case SDL_JOYAXISMOTION: {
          printf("joy axis motion\n");
          break;
        }
        case SDL_JOYBALLMOTION: {
          printf("joy ball motion\n");
        break;
        }
        case SDL_JOYHATMOTION: {
        }
        case SDL_CONTROLLERBUTTONDOWN: {
          printf("controller button downn\n");
        break;
        }
        case SDL_CONTROLLERBUTTONUP: {
          printf("controller butotn down\n");
        break;
        }
        case SDL_CONTROLLERAXISMOTION: {
          printf("controller axis motion\n");
        break;
        }
        break;
        case SDL_QUIT:
          keySelectionMode = false;
        break;
        default:
        break;
      }

    }
  }

  if(controllers[0]->copyMode){
    printf("pushing input history to inputHistoryCopy\n");
    controllers[0]->inputHistoryCopy.push_back(controllers[0]->inputHistory.front());
    controllers[0]->inputStateCopy.push_back(controllers[0]->currentState);
  }
  if (controllers[1]->playbackMode && controllers[1]->playbackCounter == controllers[0]->inputHistoryCopy.size()) {
    controllers[1]->playbackCounter = 0;
    controllers[1]->playbackMode = false;
    printf("stopping playback mode\n");
  }
}

void InputManager::writeConfig(){
  int itemCounter = 0;
  std::vector<std::string> removalKeys;
  for (auto i : configJson.items()) {
    int user = i.value().at("user");
    std::string str = i.key();
    const char* ptr = str.c_str();
    printf("the key:%s the user for this item: %d\n", ptr, user);
    if(user == userBeingConfig){
      removalKeys.push_back(i.key());
    }
  }
  
  for (auto i : removalKeys) {
    configJson.erase(i);
    printf("removalKey:%s \n", i.c_str());
  }

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

void InputManager::writeButtonConfig(){
  int itemCounter = 0;
  nlohmann::json* currentConfig = userBeingConfig == 1 ? &p1configJson : &p2configJson;
  std::vector<std::string> removalKeys;
  for (auto i : currentConfig->items()) {
    std::string str = i.key();
    const char* ptr = str.c_str();
    printf("the key:%s the user for this item: %d\n", ptr, userBeingConfig);
    removalKeys.push_back(i.key());
  }
  
  for (auto i : removalKeys) {
    currentConfig->erase(i);
    printf("removalKey:%s \n", i.c_str());
  }

  for(auto input : buttonTemplate){
    nlohmann::json obj = nlohmann::json::object();
    obj["user"] = userBeingConfig;
    std::stringstream stream;
    stream << std::hex << input;
    std::string result( stream.str() );

    obj["input"] = "0x" + result;
    printf("the input:%x\n", input);
    (*currentConfig)[std::to_string(buttonConfigArray[itemCounter])] = obj;

    itemCounter++;
    if(itemCounter == 4){
      printf("breaking button conf\n");
      break;
    }
  }

  const char* confPath = userBeingConfig == 1 ? "../data/p1buttonconf.json" : "../data/p2buttonconf.json";
  std::ofstream newConfigFile(confPath);
  (*currentConfig) >> newConfigFile;
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
