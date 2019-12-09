#ifndef _FightState_h
#define _FightState_h

#include "states/GameState.h"
#include "game_objects/Stage.h"
#include "game_objects/Character.h"
#include "screens/FightScreen.h"
#include "character_state/CharStateManager.h"
#include "graphics/Camera.h"

struct FightStateValues {
  // global values
  int stateTime;
  int screenFreeze;
  // playerValues
  int healthValues[2];
  int charStateNum[2];
  int charXPosition[2];
  int charYPosition[2];
  int charComboCount[2];
};

class FightState : public GameState {
public:
  FightState();
  ~FightState();

  void enter();
  void exit();

  void pause();
  void resume();
  void checkPushCollisions();
  void checkThrowCollisions();
  void checkHitCollisions();
  void checkEntityHitCollisions();
  void checkBounds();
  void checkHealth();
  bool checkBlock(int blockType, Character* player);
  void updateFaceRight();
  void renderHealthBars();
  void renderComboCount();
  void renderInputHistory();
  void renderHealthBar(int x, int y, int w, int h, float percent, SDL_Color fgColor, SDL_Color bgColor);

  void handleInput();
  void update();
  void draw();

  int screenFreeze = 0;
  int stateTime = 0;
  bool everythingCompiled = false;
private:
  Character* player1;
  Character* player2;
  FightScreen currentScreen;
  Camera camera;
  CharStateManager* charStateManager = CharStateManager::getInstance();
  Graphics* graphics = Graphics::getInstance();
};
#endif
