#include "character_state/CharStateManager.h"
#include "game_objects/Character.h"

void CharStateManager::registerCharacter(Character* character, int playerNum){ 
  characterPointers[playerNum-1] = character;
}

Character* CharStateManager::getCharPointer(int playerNum){ 
  return characterPointers[playerNum-1];
}
