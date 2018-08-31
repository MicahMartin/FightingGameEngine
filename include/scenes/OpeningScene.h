#ifndef _OpeningScene_h
#define _OpeningScene_h

#include <vector>
#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"
#include "Scene.h"

class OpeningScene : public Scene{
public:
  OpeningScene(Graphics* graphics);
  ~OpeningScene();

  void init();
  void update();
  void draw();

  void addTexture(GameTexture* gText);
  void removeTexture(int index);

private:
  std::vector<GameTexture*> textureList;
  Graphics* graphics;
  SDL_Renderer* renderer;
  /* data */
};
#endif /* ifndef  _openingScne_h */
