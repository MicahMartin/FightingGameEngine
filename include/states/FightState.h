#ifndef _FightState_h
#define _FightState_h

#include "states/GameState.h"
#include "game_objects/Stage.h"
#include "game_objects/Popup.h"
#include "game_objects/Character.h"
#include "screens/FightScreen.h"
#include "character_state/CharStateManager.h"
#include "graphics/Camera.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <ggponet.h>

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

struct FightStateObj {
  int p1RoundsWon;
  int p2RoundsWon;
  int currentRound;
  int roundWinner;
  int slowDownCounter;
  int roundStartCounter;
  int screenFreezeCounter;
  int screenFreezeLength;
  int frameCount;

  bool roundStart;
  bool inSlowDown;
  bool slowMode;
  bool roundEnd;
  bool screenFreeze;
  bool shouldUpdate;

  CameraStateObj cameraState;
  CharStateObj char1State;
  CharStateObj char2State;
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

  //ggpo stuff
  bool beginGame(const char* name);
  void advanceFrame();
  void saveState(unsigned char** buffer, int* length, int frame);
  void saveState();
  void freeBuffer(void* buffer);
  void loadState(unsigned char* buffer, int length);
  void loadState();

  void checkPushCollisions();
  void checkThrowCollisions();
  void checkHitCollisions();
  void checkProximityCollisions();
  void checkEntityHitCollisions();
  void checkBounds();
  void checkHealth();
  bool checkBlock(int blockType, Character* player);
  void updateFaceRight();
  void drawHealthBars();
  void drawComboCount();
  void drawInputHistory();
  void drawHealthBar(int x, int y, int w, int h, float percent, SDL_Color fgColor, SDL_Color bgColor);
  void handleRoundStart();
  void checkThrowTechs();
  void updateVisuals();
  void updateCamera();

  void checkCorner(Character* player);
  void checkHitstop(Character* player);
  void checkEntityHitstop(Character* player);

  HitResult checkHitboxAgainstHurtbox(Character* hitter, Character* hurter);
  int checkProximityAgainst(Character* hitter, Character* hurter);

  HitResult checkEntityHitAgainst(Character* thrower, Character* throwee);
  ThrowResult checkThrowAgainst(Character* thrower, Character* throwee);
  void handleSameFrameThrowTech(SpecialState techState);
  int checkProjectileCollisions(Character* player1, Character* player2);

  void drawText();
  void drawCounterHit();
  void drawUIEffects();
  void drawPlayers();
  void handleSoundEffects();



  bool roundStart = false;
  int roundStartCounter = 0;
  int p1RoundsWon = 0;
  int p2RoundsWon = 0;
  int currentRound = 0;

  bool everythingCompiled = false;
  bool inSlowDown = false;
  bool roundEnd = false;
  int slowDownCounter = 0;
  int roundWinner = 0;
  bool screenFreeze = false;
  int screenFreezeCounter = 0;
  int screenFreezeLength = 0;
  long frameCount = 0;

  Mix_Music* bgMusic = NULL;
  Mix_Chunk* yawl_ready = NULL;
  Mix_Chunk* countah = NULL;
  Mix_Chunk* instantBlock = NULL;
  Mix_Chunk* throwtech = NULL;
  Mix_Chunk* pushBlock = NULL;
  Mix_Chunk* round1Sound = NULL;
  Mix_Chunk* round2Sound = NULL;
  Mix_Chunk* finalRoundSound = NULL;
  Mix_Chunk* fightSound = NULL;
  Mix_Chunk* koSound = NULL;
  Mix_Chunk* p1WinSound = NULL;
  Mix_Chunk* p2WinSound = NULL;
  unsigned char* mostRecentState;

  unsigned char* buffer;
  int bufferLen;
  Character* player1;
  Character* player2;

  // GGPO
  void ggpoInit();
  void netPlayHandleInput();
  GGPOSession* ggpo = NULL;
  GGPOPlayer p1, p2;
  GGPOPlayerHandle player_handles[2];
  GGPOPlayerHandle* local_player_handle;
  bool shouldUpdate = true;
  bool doneSync = false;
  bool inAdvanceState = false;
  bool netPlayState;
private:
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
  int pnum;
};
#endif
