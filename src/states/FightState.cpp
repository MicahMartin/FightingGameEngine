#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"

FightState::FightState(){ }

FightState::~FightState(){ }


void FightState::enter(){ 
  // init all fields
  
  printf("entered the fight state \n");
  player1 = new Character(std::make_pair(140,0));
  player2 = new Character(std::make_pair(340,0));
  currentScreen = new FightScreen();
};

void FightState::exit(){ 
  delete player1;
  delete player2;
  delete currentScreen;
};

void FightState::pause(){ };
void FightState::resume(){ };

void FightState::handleInput(VirtualController* input){ };
void FightState::update(){ };

void FightState::draw(){  
  currentScreen->draw();
  player1->draw();
  player2->draw();
};
