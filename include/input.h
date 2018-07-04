#ifndef _input_h
#define _input_h
#include <SDL2/SDL.h>

// not sure what to use this class for anymore
class Input{
public:
  Input();
  ~Input();
  uint32_t  getKeyCode();

private:
  const char* keyName;
};

#endif /* ifndef _input_h */
