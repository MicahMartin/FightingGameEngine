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
  // playerValues
  int healthValues[2];
  int charStateNum[2];
  int charXPosition[2];
  int charYPosition[2];
  int charComboCount[2];
};

struct ThrowResult {
  bool thrown;
  CollisionBox* throwCb;
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

  void checkCorner(Character* player);
  void checkHitstop(Character* player);
  void checkEntityHitstop(Character* player);

  int checkHitboxAgainstHurtbox(Character* hitter, Character* hurter);
  ThrowResult checkThrowAgainst(Character* thrower, Character* throwee);
  int checkEntityHitAgainst(Character* thrower, Character* throwee);


  void handleInput();
  void update();
  void draw();
  void restartRound();
  bool roundStart = false;
  int roundStartCounter = 0;
  int p1RoundsWon = 0;
  int p2RoundsWon = 0;

  int playHitSound = 0;
  int playHurtSound = 0;
  int playHitSoundID;
  int playHurtSoundID;
  bool everythingCompiled = false;
  bool inSlowDown = false;
  bool shouldUpdate = true;
  bool roundEnd = false;
  int slowDownCounter = 0;

  Mix_Music* bgMusic = NULL;
private:
  Character* player1;
  Character* player2;
  FightScreen currentScreen;
  Camera camera;
  CharStateManager* charStateManager = CharStateManager::getInstance();
  Graphics* graphics = Graphics::getInstance();
};
#endif
