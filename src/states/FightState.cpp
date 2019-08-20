#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"

FightState::FightState(){ }

FightState::~FightState(){ }


void FightState::enter(){ 
  // init all fields
  
  printf("entered the fight state \n");
  player1 = new Character();
  player2 = new Character();
  currentScreen = new FightScreen();
};

void FightState::exit(){ 
  delete player1;
  delete player2;
};

void FightState::pause(){ };
void FightState::resume(){ };
void FightState::handleInput(){ };
void FightState::update(){ };

void FightState::draw(){  
  currentScreen->draw();
};
