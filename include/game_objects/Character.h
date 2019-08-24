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

  void handleInput();
  void update();
  void draw();

  // position stuff
  std::pair<int,int> getPos();
  void setX(int x);
  void setY(int y);

  // getters for these guys
  bool faceRight;
  VirtualController* virtualController;
private:
  std::vector<StateDef> stateList;

  StateDef* currentState;
  std::pair<int, int> position;
  int playerNum;
  int health;
};

#endif
