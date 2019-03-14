#ifndef _FightScreen_h
#define _FightScreen_h

#include "screens/Screen.h"

class FightScreen : public Screen {
public:
  FightScreen();
  ~FightScreen();

  void init();
  void update();
  void draw();

  void addTexture(GameTexture* gText);
  void removeTexture(int index);

private:
};

#endif
