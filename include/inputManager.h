#ifndef _inputManager_h
#define _inputManager_h 

#include <SDL2/SDL.h>

class InputManager{

public:
  InputManager();
  ~InputManager();
  void init();

private:
  /* data */
  SDL_Event event;
};

#endif /* ifndef _inputManager_h */
