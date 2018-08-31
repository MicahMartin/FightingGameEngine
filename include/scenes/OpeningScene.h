#ifndef _OpeningScene_h
#define _OpeningScene_h

#include "Scene.h"
#include "graphics/GameTexture.h"
#include <vector>

class OpeningScene : public Scene{
public:
  OpeningScene();
  ~OpeningScene();
  void init();
  void update();

  void addTexture(GameTexture* gText);
  void removeTexture(int index);

private:
  std::vector<GameTexture*> textureList;
  /* data */
};
#endif /* ifndef  _openingScne_h */
