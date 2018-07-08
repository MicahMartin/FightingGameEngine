#ifndef _graphics_h
#define _graphics_h

#include <SDL2_image/SDL_image.h>
//#include <SDL2/SDL_opengl.h>

// handle SDL init, window stuff, and video buffer swapping at the end of every frame
class Graphics{
public:
  Graphics();
  ~Graphics();
  void init(int w, int h);
  void update();

  void resizeWindow(int w, int h);
  static SDL_Texture* loadTexture(const char* path, SDL_Renderer* rend);

  int getWindowWidth();
  int getWindowHeight();
  // SDL_Surface* getWindowSurface();
  //void perspectiveGl(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
  //void renderBackGround(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  //GLuint imgToTexture(const char *imageLocation);
private:
  int width;
  int height;
  SDL_Window* window;
  SDL_Renderer* renderer;
  // The sdlSurface is a data type with the pixel data 
  // and some other data of an image
  // SDL_Surface* windowSurface;

};


#endif
