#ifndef _GameTexture_h
#define _GameTexture_h 
#include <SDL2_image/SDL_image.h>

class GameTexture{
public:
  GameTexture();
  ~GameTexture();

  bool loadTexture(const char* path, SDL_Renderer* rend);
  void render(SDL_Renderer* rend);

  void setDimensions(int xCord, int yCord, int width, int height);
  std::pair<int, int> getDimensions();
  std::pair<int, int> getCords();

private:
  SDL_Texture* texture;
  SDL_Rect textRect;
  int xCord, yCord, width, height;
  /* data */
};
#endif /* ifndef _GameTexture_h */
