#ifndef _FightState_h
#define _FightState_h

#include "states/GameState.h"
#include "game_objects/Stage.h"
#include "game_objects/Popup.h"
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

struct HitResult {
  bool hit;
  bool counter;
  int hitState;
  CollisionBox* hitCb;
};

class FightState : public GameState {
public:
  FightState();
  ~FightState();

  void enter();
  void exit();

  void handleInput();
  void update();
  void draw();
  void restartRound();

  void pause();
  void resume();

  void checkPushCollisions();
  void checkThrowCollisions();
  void checkHitCollisions();
  void checkProximityCollisions();
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

  HitResult checkHitboxAgainstHurtbox(Character* hitter, Character* hurter);
  int checkProximityAgainst(Character* hitter, Character* hurter);

  HitResult checkEntityHitAgainst(Character* thrower, Character* throwee);
  ThrowResult checkThrowAgainst(Character* thrower, Character* throwee);
  int checkProjectileCollisions(Character* player1, Character* player2);



  bool roundStart = false;
  int roundStartCounter = 0;
  int p1RoundsWon = 0;
  int p2RoundsWon = 0;
  int currentRound = 0;

  bool everythingCompiled = false;
  bool inSlowDown = false;
  bool shouldUpdate = true;
  bool roundEnd = false;
  int slowDownCounter = 0;
  int roundWinner = 0;

  Mix_Music* bgMusic = NULL;
  Mix_Chunk* yawl_ready = NULL;
  Mix_Chunk* countah = NULL;
  Mix_Chunk* instantBlock = NULL;
  Mix_Chunk* round1Sound = NULL;
  Mix_Chunk* round2Sound = NULL;
  Mix_Chunk* finalRoundSound = NULL;
  Mix_Chunk* fightSound = NULL;
  Mix_Chunk* koSound = NULL;
  Mix_Chunk* p1WinSound = NULL;
  Mix_Chunk* p2WinSound = NULL;
private:
  Character* player1;
  Character* player2;
  FightScreen currentScreen;

  Popup matchIntroPopup;
  Popup round1;
  Popup round2Popup;
  Popup finalRoundPopup;

  Popup fightPopup;
  Popup knockoutPopup;
  Popup p1WinPopup;
  Popup p2WinPopup;

  Popup p1CounterHit;
  Popup p2CounterHit;

  Camera camera;
  CharStateManager* charStateManager = CharStateManager::getInstance();
  Graphics* graphics = Graphics::getInstance();
};
#endif
