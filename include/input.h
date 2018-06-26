#ifndef _input_h
#define _input_h
#include <SDL2/SDL.h>

class Input{
public:
  Input();
  Input(SDL_Event event);
  ~Input();

private:
  SDL_Event event;

};

#endif /* ifndef _input_h */
