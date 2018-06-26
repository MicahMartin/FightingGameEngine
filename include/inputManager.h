#ifndef _inputManager_h
#define _inputManager_h 

#include <SDL2/SDL.h>
#include <vector>
#include "input.h"


class InputManager{

public:
  InputManager();
  ~InputManager();
  void init();
  void update();

private:
  /* data */
  SDL_Event event;
  // we'll probably have to timestamp when an item is added to the inputlist
  // to make it easier to manage leniency
  std::vector<Input> inputList;
};

#endif /* ifndef _inputManager_h */
