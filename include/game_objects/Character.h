#ifndef _Character_h
#define _Character_h 

#include "character_state/StateDef.h"
#include "character_state/CharacterState.h"
#include "input/VirtualController.h"
#include "graphics/GameTexture.h"

class Character {
public:
  Character(std::pair<int, int> position, int health);
  Character(std::pair<int, int> position);
  void init();

  ~Character();

  void handleInput(VirtualController* controller);
  void update();
  void draw();

private:
  std::pair<int, int> position;
  CharacterState* currentState;
  GameTexture* currentSprite;
  int health;
};

#endif
