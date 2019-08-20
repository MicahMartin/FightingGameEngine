#ifndef _Graphics_h
#define _Graphics_h

#include <SDL2_image/SDL_image.h>

// handle SDL init, window stuff, and video buffer swapping at the end of every frame
class Graphics {
public:
  Graphics(int w, int h);
  ~Graphics(); 

  void clear();
  void present();

  void resizeWindow(int w, int h);

  SDL_Renderer* getRenderer();
  SDL_Window* getWindow();
  int getWindowWidth();
  int getWindowHeight();

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  int width;
  int height;
};

#endif
