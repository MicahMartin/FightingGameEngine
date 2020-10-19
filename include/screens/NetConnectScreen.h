#ifndef _NetConnectScreen_h
#define _NetConnectScreen_h

#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"
#include "Screen.h"

class NetConnectScreen : public Screen {
public:
  NetConnectScreen();
  ~NetConnectScreen();

  void init();
  void update();
  void draw();

private:
  GameTexture menuBackground;
  int screenTime;
};
#endif
