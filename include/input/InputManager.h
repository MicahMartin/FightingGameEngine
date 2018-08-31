#ifndef _InputManager_h
#define _InputManager_h 

#include <SDL2/SDL.h>
#include <vector>
#include <unordered_map>
#include <map>
#include "pugixml.hpp"
#include "observer/Observer.h"
#include "observer/Subject.h"
#include "input/VirtualController.h"



class InputManager : public Subject{

public:
  uint16_t inputBits;
  InputManager();
  ~InputManager();
  // runnable?
  void init();
  void update();

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
  
  // button config
  pugi::xml_document config;
  std::unordered_map<uint32_t, uint16_t>  bConf;

  // would need to be careful about observing deleted objects since using pointers
  std::map<const char*, Observer*> observerList;
  int numOfObservers;
};


#endif /* ifndef _inputManager_h */
