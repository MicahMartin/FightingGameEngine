#ifndef _Graphics_h
#define _Graphics_h

#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>

// handle SDL init, window stuff, and video buffer swapping at the end of every frame
enum GraphicsFlag {
  GF_SHOW_CB = 0x1
};

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

  void clearFlag(GraphicsFlag flag);
  void setFlag(GraphicsFlag flag);
  bool getFlag(GraphicsFlag flag);

  void clear();
  void present();

  void resizeWindow(bool fullScreen);
  void resizeWindow(int w, int h);
  void setFPS(int _fps);
  int getFPS();

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
  int flags = 0;
  int fps = 0;
  TTF_Font* scpFont;
  // The sdlSurface is a data type with the pixel data 
  // and some other data of an image
  // SDL_Surface* windowSurface;

};


#endif
