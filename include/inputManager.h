#ifndef _inputManager_h
#define _inputManager_h 

#include <SDL2/SDL.h>
#include <vector>
#include "input.h"
#include "observer.h"
#include "subject.h"
#include <experimental/optional>


class InputManager : public Subject{

public:
  InputManager();
  ~InputManager();
  void init();
  void update();
  std::experimental::optional<Input> getLastInput();
  int getInputHistorySize();

  void addObserver(Observer* observer);
  void removeObserver(Observer* observer);
  void notifyAll(const char* messageType);

private:
  /* data */
  // we'll probably have to timestamp when an item is added to the inputlist
  // to make it easier to manage leniency
  Input inputEvent;
  std::vector<Input> inputList;
  std::vector<Input> inputHistory;
  // would need to be careful about observing deleted objects  since using pointers
  // key value list might be better here since we'd be doing a lot of random access?
  std::vector<Observer*> observerList;
  int numOfObservers;
};

#endif /* ifndef _inputManager_h */
