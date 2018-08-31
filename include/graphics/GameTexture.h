#ifndef _GameTexture_h
#define _GameTexture_h 
#include <SDL2_image/SDL_image.h>

class GameTexture{
public:
  GameTexture();
  ~GameTexture();

  bool loadTexture(const char* path, SDL_Renderer* rend);
  void render(int x, int y);

private:
  SDL_Texture* texture;

  int width;
  int height;
  /* data */
};
#endif /* ifndef _GameTexture_h */
