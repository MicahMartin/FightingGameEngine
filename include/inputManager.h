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
};

#endif /* ifndef _inputManager_h */
