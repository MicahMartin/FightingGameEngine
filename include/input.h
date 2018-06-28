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
    UPRIGHT = 1 << 4,
    UPLEFT = 1 << 5,
    DOWNRIGHT = 1 << 6,
    DOWNLEFT = 1 << 7,
    LIGHTP = 1 << 8,
    LIGHTK = 1 << 9,
    MEDIUMP = 1 << 10,
    MEDIUMK = 1 << 11,
    HARDP = 1 << 12,
    HARDK = 1 << 13,
    SELECT = 1 << 14,
    START = 1 << 15,
    ANYP = 1 << 16,
    ANYK = 1 << 17,
    LEFTANDRIGHT = 1 << 18,
    ANYRIGHT = 1 << 19,
    ANYLEFT = 1 << 20,
    ANYUP = 1 << 21,
    ANYDOWN = 1 << 22,
    NOTHING = 1 << 23,
    MISC1 = 1 << 24,
    MISC2 = 1 << 25,
  };

private:
  const char* keyName;
  uint32_t inputEnum = NEUTRAL;

};

#endif /* ifndef _input_h */
