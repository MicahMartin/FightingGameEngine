#ifndef _input_h
#define _input_h
#include <SDL2/SDL.h>




class Input{
public:
  Input();
  //Input(SDL_Event event);
  ~Input();
  uint32_t  getKeyCode();
  void setBits(SDL_Event event);
  void clearBits(SDL_Event event);

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

private:
  const char* keyName;
  uint32_t inputEnum = NEUTRAL;

};

#endif /* ifndef _input_h */
