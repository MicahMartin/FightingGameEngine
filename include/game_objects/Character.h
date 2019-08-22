#ifndef _Character_h
#define _Character_h 

#include "character_state/StateDef.h"
#include "input/VirtualController.h"

class Character {
public:
  Character(std::pair<int, int> position, int playerNum);
  Character(std::pair<int, int> position);

  ~Character();

  void loadStates();
  void changeState(int stateDefNum);

  void handleInput(VirtualController* controller);
  void update();
  void draw();

  std::pair<int,int> getPos();
  bool faceRight;
private:
  std::vector<StateDef> stateList;

  StateDef* currentState;
  std::pair<int, int> position;
  int playerNum;
  int health;
};

#endif
