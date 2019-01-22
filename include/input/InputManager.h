#ifndef _InputManager_h
#define _InputManager_h 

#include <SDL2/SDL.h>
#include <unordered_map>
#include <map>
#include "pugixml.hpp"
#include "observer/Observer.h"
#include "observer/Subject.h"
#include "input/VirtualController.h"



class InputManager : public Subject{

public:
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

  enum Input { 
    NOINPUT = 0,

    DOWN = 0x1,
    RIGHT = 0x2,
    LEFT = 0x4,
    UP = 0x8,

    DOWNLEFT = (DOWN | LEFT),
    DOWNRIGHT = (DOWN | RIGHT),
    UPLEFT = (UP | LEFT),
    UPRIGHT = (UP | RIGHT),

    LP  = 0x10,
    MP = 0x20,
    HP = 0x40,
    AP = 0x80,
    // TODO: ANYP
    // TODO: MULTIP

    LK  = 0x100,
    MK = 0x200,
    HK = 0x400,
    AK = 0x800,
    // TODO: ANYK
    // TODO: MULTIK

    START = 0x1000,
    SELECT = 0x2000,
    MISC1 = 0x4000,
    MISC2 = 0x8000,
  };


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
