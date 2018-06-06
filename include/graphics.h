#ifndef _graphics_h
#define _graphics_h

#include <SDL2/SDL_opengl.h>

class Graphics
{
   private:
      bool global_loaded;
      int width;
      int height;
   public:
      Graphics();
      ~Graphics();
      void init(int w, int h);
      void resizeWindow(int w, int h);
      void perspectiveGl(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
      int  getScreenWidth();
      int  getScreenHeight();
};


#endif
