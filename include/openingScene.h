#ifndef _openingScene_h
#define _openingScene_h
#include "scene.h"


class OpeningScene : public Scene{
public:
  OpeningScene();
  ~OpeningScene();
  void init();
  void update();

  SDL_Surface* getCurrentSurface();
  void setSurface(SDL_Surface* surface);

private:
  SDL_Surface* currentSurface;
  /* data */
};
#endif /* ifndef  _openingScne_h */
