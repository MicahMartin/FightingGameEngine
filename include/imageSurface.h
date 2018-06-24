#ifndef _imageSurface_h
#define _imageSurface_h

#include <SDL2_image/SDL_image.h>

class ImageSurface{
public:
  ImageSurface();
  ~ImageSurface();

  bool loadImg(const char* imgPath);
  void freeSurface();
  void blitImg(SDL_Surface* surface);

private:
  SDL_Surface* imgBmp;
  /* data */
};
#endif
