#ifndef _Graphics_h
#define _Graphics_h

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

  SDL_Renderer* getRenderer();
  SDL_Window* getWindow();
  int getWindowWidth();
  int getWindowHeight();
  // SDL_Surface* getWindowSurface();
  //void perspectiveGl(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
  //void renderBackGround(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
  //GLuint imgToTexture(const char *imageLocation);
private:
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;
  int width;
  int height;
  // The sdlSurface is a data type with the pixel data 
  // and some other data of an image
  // SDL_Surface* windowSurface;

};


#endif
