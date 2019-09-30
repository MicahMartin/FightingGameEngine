#ifndef _FightState_h
#define _FightState_h

#include "states/GameState.h"
#include "game_objects/Stage.h"
#include "game_objects/Character.h"
#include "screens/FightScreen.h"
#include "character_state/CharStateManager.h"

class FightState : public GameState {
public:
  FightState();
  ~FightState();

  void enter();
  void exit();

  void pause();
  void resume();
  void renderHealthBar(int x, int y, int w, int h, float percent, SDL_Color fgColor, SDL_Color bgColor);

  void handleInput();
  void update();
  void draw();

  int screenFreeze = 0;
private:
  Character* player1;
  Character* player2;
  FightScreen* currentScreen;
  CharStateManager* charStateManager = CharStateManager::getInstance();
  Graphics* graphics = Graphics::getInstance();
};
#endif
