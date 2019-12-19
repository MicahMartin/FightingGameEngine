#ifndef _InputManager_h
#define _InputManager_h 

#include <SDL2/SDL.h>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>
#include "observer/Observer.h"
#include "observer/Subject.h"
#include "input/VirtualController.h"

struct ConfItem {
  uint8_t user;
  Input inputBit;
};

typedef std::unordered_map<uint32_t, ConfItem> ConfT;
class InputManager : public Subject {

public:
  static InputManager* getInstance(){
    static InputManager instance;
    return &instance;
  };
  // runnable?
  void init();
  void initConfig(const char* fileName, ConfT* config, nlohmann::json* confJson);
  void update();
  void writeConfig();
  void writeButtonConfig();

  void addVirtualController(VirtualController* controller);
  VirtualController* getVirtualController(int index);

  // Subject
  void addObserver(const char* observerName, Observer* observer);
  void removeObserver(const char* observerName);
  void notifyAll(const char* eventName);
  void notifyOne(const char* observerName, const char* eventName);

  bool keySelectionMode = false;
  int configCounter = 0;
  uint32_t configArray[8];
  int userBeingConfig = 0;
  int inputTemplate[8] = {
    0x1,
    0x2,
    0x4,
    0x8,
    0x10,
    0x100,
    0x20,
    0x200 
  };
  uint32_t buttonConfigArray[4];
  int buttonTemplate[4] = {
    0x10,
    0x100,
    0x20,
    0x200 
  };
  SDL_Joystick* sdlController = NULL;
  std::unordered_map<SDL_Joystick*, VirtualController*> stickToVC;
private:
  InputManager() = default;
  ~InputManager() = default;
  InputManager(const InputManager&) = delete;
  InputManager& operator=(const InputManager&) = delete;
  InputManager(InputManager&&) = delete;
  InputManager& operator=(InputManager&&) = delete;
  std::vector<VirtualController*> controllers;
  
  // button config
  nlohmann::json configJson;
  nlohmann::json p1configJson;
  nlohmann::json p2configJson;

  std::unordered_map<uint32_t, ConfItem> bConf;
  std::unordered_map<uint32_t, ConfItem> p1bConf;
  std::unordered_map<uint32_t, ConfItem> p2bConf;

  // would need to be careful about observing deleted objects since using pointers
  std::unordered_map<const char*, Observer*> observerList;
  int numOfObservers;
};


#endif /* ifndef _inputManager_h */
