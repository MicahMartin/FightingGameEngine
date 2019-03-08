#ifndef _Graphics_h
#define _Graphics_h

#include <SDL2_image/SDL_image.h>
//#include <SDL2/SDL_opengl.h>

// handle SDL init, window stuff, and video buffer swapping at the end of every frame
class Graphics final {
public:
  static Graphics& getInstance(){
    static Graphics instance;
    return instance;
  };

  void init(int w, int h);
  void update();

  void clear();
  void present();

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
  Graphics() = default;
  ~Graphics(){
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    IMG_Quit();
    SDL_Quit();
  };
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics(Graphics&&) = delete;
  Graphics& operator=(Graphics&&) = delete;

  SDL_Window* window;
  SDL_Renderer* renderer;
  int width;
  int height;
  // The sdlSurface is a data type with the pixel data 
  // and some other data of an image
  // SDL_Surface* windowSurface;

};


#endif
