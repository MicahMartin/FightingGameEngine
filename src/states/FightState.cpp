#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"

FightState::FightState(){ }

FightState::~FightState(){ }


void FightState::enter(){ 
  // init all fields
  
  printf("entered the fight state \n");
  player1 = new Character(std::make_pair(500,0), 1);
  player2 = new Character(std::make_pair(780,0), 2);
  player1->virtualController = inputManager->getVirtualController(0);
  player2->virtualController = inputManager->getVirtualController(1);
  
  charStateManager->registerCharacter(player1, 1);
  charStateManager->registerCharacter(player2, 2);

  currentScreen = new FightScreen();
};

void FightState::exit(){ 
  delete player1;
  delete player2;
  delete currentScreen;
};

void FightState::pause(){ };
void FightState::resume(){ };

void FightState::handleInput(){ 
  if(player1->getPos().first <= player2->getPos().first){
    if(!player1->currentState->checkFlag(StateDef::NO_TURN)){
      player1->faceRight = true;
    }
    if(!player2->currentState->checkFlag(StateDef::NO_TURN)){
      player2->faceRight = false;
    }
  } else {
    if(!player1->currentState->checkFlag(StateDef::NO_TURN)){
      player1->faceRight = false;
    }
    if(!player2->currentState->checkFlag(StateDef::NO_TURN)){
      player2->faceRight = true;
    }
  }
  player1->handleInput();
  player2->handleInput();
};
void FightState::update(){ 
  player1->update();
  player2->update();
};

void FightState::draw(){  
  currentScreen->draw();
  player1->draw();
  player2->draw();
};
