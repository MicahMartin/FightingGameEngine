#ifndef _FightScreen_h
#define _FightScreen_h

#include "screens/Screen.h"
#include "input/VirtualController.h"

struct HealthBar {
  int healthValue;
  GameTexture bar;
  GameTexture health;
  GameTexture damage;
};

class FightScreen : public Screen {
public:
  FightScreen();
  ~FightScreen();

  void init();
  void update();
  void draw();

  void addTexture(GameTexture* gText);
  void removeTexture(int index);

  void renderHealthBar(float healthPercent, float damagePercent,  bool isPlayerOne);
  void renderComboCount(bool side, int count);
  void renderInputHistory(bool side, boost::circular_buffer<InputEvent>& events);
  void renderWins(int p1Wins, int p2Wins);
private:
  int foobar = 0;
  int barfoo = 2175;
  GameTexture stage;
  GameTexture sky;
  GameTexture numbers[10];
  GameTexture directions[4];
  GameTexture buttons[8];

  GameTexture roundBoxes[4];
  GameTexture roundWinBoxes[4];

  SDL_Rect p1ComboCountPosition;
  SDL_Rect p2ComboCountPosition;
  SDL_Rect p1ComboCountPositionSecond;
  SDL_Rect p2ComboCountPositionSecond;

  SDL_Rect inputHistoryPositions[10] = {
    {50, 200, 50, 50},
    {50, 250, 50, 50},
    {50, 300, 50, 50},
    {50, 350, 50, 50},
    {50, 400, 50, 50},
    {50, 450, 50, 50},
    {50, 500, 50, 50},
    {50, 550, 50, 50},
    {50, 600, 50, 50},
    {50, 650, 50, 50},
  };

  SDL_Rect roundBoxPositions[4] = {
    {465, 10, 30, 30},
    {525, 10, 30, 30},
    {725, 10, 30, 30},
    {785, 10, 30, 30},
  };
  int p2DirectionDrawXPos;

  HealthBar p1HealthBar;
  HealthBar p2HealthBar;
};

#endif
