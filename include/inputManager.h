#ifndef _inputManager_h
#define _inputManager_h 

#include <SDL2/SDL.h>
#include <vector>
#include "observer.h"
#include "subject.h"
#include "virtualController.h"
#include <map>


class InputManager : public Subject{

public:
  InputManager();
  ~InputManager();
  // runnable?
  void init();
  void update();

  uint16_t getInputByte();
  int getInputHistorySize();
  VirtualController* getVirtualController();

  void setBit(SDL_Event event);
  void clearBit(SDL_Event event);

  // Subject
  void addObserver(const char* observerName, Observer* observer);
  void removeObserver(const char* observerName);
  void notifyAll(const char* eventName);
  void notifyOne(const char* observerName, const char* eventName);

  enum InputType {  
    NEUTRAL = 0,
    DOWN = 1 << 0,
    RIGHT = 1 << 1,
    LEFT = 1 << 2,
    UP = 1 << 3,
    LIGHTP = 1 << 4,
    LIGHTK = 1 << 5,
    MEDIUMP = 1 << 6,
    MEDIUMK = 1 << 7,
    HARDP = 1 << 8,
    HARDK = 1 << 9,
    ALLP = 1 << 10,
    ALLK = 1 << 12,
    SELECT = 1 << 13,
    START = 1 << 14,
    MISC = 1 << 15,
  };


private:
  uint16_t inputEnum = NEUTRAL;
  VirtualController virtualController;
  // we'll probably have to timestamp when an item is added to the inputlist
  // to make it easier to manage leniency
  // Input inputEvent;
  
  // would need to be careful about observing deleted objects since using pointers
  std::map<const char*, Observer*> observerList;
  int numOfObservers;
};

#endif /* ifndef _inputManager_h */
