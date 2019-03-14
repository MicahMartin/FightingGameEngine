#ifndef _Character_h
#define _Character_h 

#include <vector>
#include "character_state/StateDef.h"

class Character {
public:
  Character();
  ~Character();

private:
  StateDef currentState;
  std::vector<int, int> position;
};

#endif
