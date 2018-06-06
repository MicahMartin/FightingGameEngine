#ifndef _graphics_h
#define _graphics_h

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>

class Graphics
{
   private:
      int width;
      int height;
   public:
      Graphics();
      ~Graphics();
      void init(int w, int h);
      void resizeWindow(int w, int h);
      void perspectiveGl(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
      void render();
      int  getScreenWidth();
      int  getScreenHeight();
};


#endif
