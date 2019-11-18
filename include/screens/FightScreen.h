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

  void renderHealthBar(int x, int y, int w, int h, float percent, SDL_Color fgColor, SDL_Color bgColor);
  void renderComboCount(bool side, int count);
  void renderInputHistory(bool side);
private:
  GameTexture stage;
  GameTexture numbers[10];
  SDL_Rect p1ComboCountPosition;
  SDL_Rect p2ComboCountPosition;
  SDL_Rect p1ComboCountPositionSecond;
  SDL_Rect p2ComboCountPositionSecond;
};

#endif
