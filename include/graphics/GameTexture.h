#ifndef _GameTexture_h
#define _GameTexture_h 
#include <SDL2_image/SDL_image.h>

class GameTexture{
public:
  GameTexture(SDL_Renderer* renderer);
  ~GameTexture();

  bool loadTexture(const char* path);
  void render();

  void setDimensions(int xCord, int yCord, int width, int height);
  std::pair<int, int> getDimensions();
  std::pair<int, int> getCords();

private:
  SDL_Texture* texture;
  SDL_Rect textRect;
  SDL_Renderer* renderer;
  int xCord, yCord, width, height;
  /* data */
};
#endif /* ifndef _GameTexture_h */
