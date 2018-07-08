#ifndef _openingScene_h
#define _openingScene_h
#include "scene.h"


class OpeningScene : public Scene{
public:
  OpeningScene();
  ~OpeningScene();
  void init();
  void update();

  SDL_Texture* getCurrentTexture();
  // set the address of a texture to sceneTexture
  void setTexture(SDL_Texture* texture);

private:
  SDL_Texture* sceneTexture;
  /* data */
};
#endif /* ifndef  _openingScne_h */
