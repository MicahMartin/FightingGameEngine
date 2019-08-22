#include "character_state/CharStateManager.h"

void CharStateManager::registerCharacter(Character* character, int playerNum){ 
  characterPointers[playerNum-1] = character;
}

Character* CharStateManager::getCharPointer(int playerNum){ 
  return characterPointers[playerNum-1];
}
