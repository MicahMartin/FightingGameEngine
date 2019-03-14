#ifndef _Character_h
#define _Character_h 

#include <vector>
#include "character_state/StateDef.h"

class Character {
public:
  Character();
  ~Character();

  void update();
  void draw();

private:
  std::vector<std::pair<int, int>> position;
};

#endif
