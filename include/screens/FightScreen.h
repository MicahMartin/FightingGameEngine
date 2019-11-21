#ifndef _FightScreen_h
#define _FightScreen_h

#include "screens/Screen.h"
#include "input/VirtualController.h"

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
  void renderInputHistory(bool side, boost::circular_buffer<InputEvent>& events);
private:
  GameTexture stage;
  GameTexture numbers[10];
  GameTexture directions[4];
  GameTexture buttons[8];
  SDL_Rect p1ComboCountPosition;
  SDL_Rect p2ComboCountPosition;
  SDL_Rect p1ComboCountPositionSecond;
  SDL_Rect p2ComboCountPositionSecond;
  SDL_Rect inputHistoryPositions[10] = {
    {50, 100, 50, 50},
    {50, 150, 50, 50},
    {50, 200, 50, 50},
    {50, 250, 50, 50},
    {50, 300, 50, 50},
    {50, 350, 50, 50},
    {50, 400, 50, 50},
    {50, 450, 50, 50},
    {50, 500, 50, 50},
    {50, 550, 50, 50},
  };
  int p2DirectionDrawXPos;
};

#endif
