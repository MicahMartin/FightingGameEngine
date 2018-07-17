#ifndef _openingScene_h
#define _openingScene_h

#include "scene.h"
#include "gameTexture.h"
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
