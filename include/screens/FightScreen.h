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

struct MeterBar {
  int meterValue;
  int burstValue;
  GameTexture bar;
  GameTexture meter;
  GameTexture burst;
};

enum RecordingStatus {
  RECORDING_NONE,
  RECORDING_ONE,
  RECORDING_TWO,
  PLAYBACK_ONE,
  PLAYBACK_TWO,
  CONNECTING
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
  void renderMeterBar(float meterPercent, float comebackPercent, bool isPlayerOne);
  void renderBurstBar(float burstPercent, bool isPlayerOne);
  void renderComboCount(bool side, int count);
  void renderInputHistory(bool side, boost::circular_buffer<InputEvent>& events);
  void renderWins(int p1Wins, int p2Wins);

  RecordingStatus recordStatus = RECORDING_NONE;
  bool showGradient = false;
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

  HealthBar p1HealthBar,
            p2HealthBar;

  MeterBar p1MeterBar,
           p2MeterBar;

  MeterBar p1BurstBar,
           p2BurstBar;

  GameTexture s1Record,
              s2Record,
              p1Record,
              p2Record,
              p3Record,
              connectStatus;

  bool        s1 = false,
              s2 = false,
              p1 = false,
              p2 = false,
              p3 = false;
};    
      
#endif
