#ifndef _GameTexture_h
#define _GameTexture_h 
#include <SDL2_image/SDL_image.h>
#include "Graphics.h"

class GameTexture{
public:
  GameTexture();
  ~GameTexture();

  bool loadTexture(const char* path);
  void render();

  void setDimensions(int xCord, int yCord, int width, int height);
  std::pair<int, int> getDimensions();
  std::pair<int, int> getCords();

private:
  Graphics& graphics = Graphics::getInstance();
  SDL_Texture* texture;
  SDL_Renderer* renderer = graphics.getRenderer();
  SDL_Rect textRect;
  int xCord, yCord, width, height;
  /* data */
};
#endif /* ifndef _GameTexture_h */
