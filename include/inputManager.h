#ifndef _inputManager_h
#define _inputManager_h 

#include <SDL2/SDL.h>
#include <vector>
#include "input.h"
#include "observer.h"
#include "subject.h"


class InputManager : public Subject{

public:
  InputManager();
  ~InputManager();
  // runnable?
  void init();
  void update();

  uint32_t getInputByte();
  int getInputHistorySize();

  void setBits(SDL_Event event);
  void clearBits(SDL_Event event);

  // Subject
  void addObserver(Observer* observer);
  void removeObserver(Observer* observer);
  void notifyAll(const char* messageType);


private:
  enum InputType {  
    NEUTRAL = 0,
    DOWN = 1 << 0,
    RIGHT = 1 << 1,
    LEFT = 1 << 2,
    UP = 1 << 3,
    // DOWNRIGHT = 0000 0011 ( 3 )
    // DOWNLEFT = 0000 0101 ( 5 )
    //
    // UPRIGHT = 0000 1010 ( 10 )
    // UPLEFT = 0000 1100 ( 12 )
    LIGHTP = 1 << 4,
    LIGHTK = 1 << 5,
    MEDIUMP = 1 << 6,
    MEDIUMK = 1 << 7,
    HARDP = 1 << 8,
    HARDK = 1 << 9,
    ALLP = 1 << 10,
    ALLK = 1 << 11,
    SELECT = 1 << 11,
    START = 1 << 12,
  };
  uint32_t inputEnum = NEUTRAL;
  // we'll probably have to timestamp when an item is added to the inputlist
  // to make it easier to manage leniency
  // Input inputEvent;
  
  // would need to be careful about observing deleted objects since using pointers
  // key value list might be better here?
  std::vector<Observer*> observerList;
  int numOfObservers;
};

#endif /* ifndef _inputManager_h */
