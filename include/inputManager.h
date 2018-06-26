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
  void init();
  void update();
  Input getLastItemFromInputArray();

  void addObserver(Observer* observer);
  void removeObserver(Observer* observer);
  void notifyAll(const char* messageType);

private:
  /* data */
  SDL_Event event;
  // we'll probably have to timestamp when an item is added to the inputlist
  // to make it easier to manage leniency
  std::vector<Input> inputList;
  // would need to be careful about observing deleted objects  since using pointers
  // key value list might be better here since we'd be doing a lot of random access?
  std::vector<Observer*> observerList;
  int numOfObservers;
};

#endif /* ifndef _inputManager_h */
