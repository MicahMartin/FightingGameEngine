#ifndef _Graphics_h
#define _Graphics_h

#include <SDL2_image/SDL_image.h>
#include "SDL_ttf.h"
#include <string>

// handle SDL init, window stuff, and video buffer swapping at the end of every frame
class Camera;
class Graphics final {
public:
  static Graphics* getInstance(){
    static Graphics instance;
    return &instance;
  };

  void init();
  void update();
  void setCamera(Camera* cam);

  void clear();
  void present();

  void resizeWindow(int w, int h);

  SDL_Renderer* getRenderer();
  SDL_Window* getWindow();
  Camera* getCamera();
  int getWindowWidth();
  int getWindowHeight();
  TTF_Font* getFont();
  std::string windowName = "BeatDown ";

private:
  Graphics() = default;
  ~Graphics(){
  };
  Graphics(const Graphics&) = delete;
  Graphics& operator=(const Graphics&) = delete;
  Graphics(Graphics&&) = delete;
  Graphics& operator=(Graphics&&) = delete;

  Camera* camera;
  SDL_Window* window;
  SDL_Renderer* renderer;
  int width;
  int height;
  TTF_Font* scpFont;
  // The sdlSurface is a data type with the pixel data 
  // and some other data of an image
  // SDL_Surface* windowSurface;

};


#endif
