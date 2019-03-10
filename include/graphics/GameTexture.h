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

  void incTransperancy();
  void decTransperancy();
  void setTransperancy(uint8_t tran);
  uint8_t getTransperancy();

  void setBlendMode(SDL_BlendMode blendmode);

  void setDimensions(int xCord, int yCord, int width, int height);
  std::pair<int, int> getDimensions();
  std::pair<int, int> getCords();

private:
  Graphics& graphics = Graphics::getInstance();
  SDL_Renderer* renderer = graphics.getRenderer();
  SDL_Texture* texture;
  SDL_Rect textRect;
  int xCord, yCord, width, height;
  uint8_t alpha = 255;
  /* data */
};
#endif /* ifndef _GameTexture_h */
