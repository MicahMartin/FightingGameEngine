#ifndef _inputManager_h
#define _inputManager_h 

#include <SDL2/SDL.h>
#include <vector>
#include <unordered_map>
#include <map>
#include "observer.h"
#include "subject.h"
#include "virtualController.h"
#include "pugixml.hpp"



class InputManager : public Subject{

public:
  InputManager();
  ~InputManager();
  // runnable?
  void init();
  void update();

  // create a key composed of 2 32 byte values from sdl event
  // uint32_t getEventKey(uint16_t eventTypeEnum, uint16_t eventTypeValue);
  uint32_t getEventValue(SDL_Event event);
  // virtual controller is gonna be owned by player object
  VirtualController* getVirtualController();

  // Subject
  void addObserver(const char* observerName, Observer* observer);
  void removeObserver(const char* observerName);
  void notifyAll(const char* eventName);
  void notifyOne(const char* observerName, const char* eventName);

private:
  // TODO: This is gonna come from the player pointer
  // Player* player;
  // VirtualController* virtualController; ...player->getVirtualController();
  VirtualController virtualController;
  // would need to be careful about observing deleted objects since using pointers
  std::map<const char*, Observer*> observerList;
  int numOfObservers;
  
  pugi::xml_document config;
  std::unordered_map<uint32_t, uint16_t>  bConf;
};


#endif /* ifndef _inputManager_h */
