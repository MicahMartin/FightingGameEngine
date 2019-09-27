#ifndef _Character_h
#define _Character_h 

#include "character_state/StateDef.h"
#include "input/VirtualController.h"
#include "character_state/StateController.h"
#include <nlohmann/json.hpp>

class Character {
public:
  Character(std::pair<int, int> position, int playerNum);
  Character(std::pair<int, int> position);
  void init();

  ~Character();

  void loadStates();
  void changeState(int stateDefNum);

  void handleInput();
  void update();
  void draw();

  // position stuff
  std::pair<int,int> getPos();
  void setXPos(int x);
  void setX(int x);
  void setY(int y);

  // getters for these guys
  VirtualController* virtualController;
  StateDef* currentState;
  bool faceRight;
  int playerNum;
  int velocityX;
  int velocityY;
  int health;

private:
  nlohmann::json stateJson;
  std::vector<StateDef> stateList;
  std::pair<int, int> position;
};

#endif
