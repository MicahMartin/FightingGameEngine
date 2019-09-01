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
    std::cout << "player1 lefter" << std::endl;
    std::cout << player1->getPos().first << " " << player2->getPos().first << std::endl;
    player1->faceRight = true;
    player2->faceRight = false;
  } else {
    std::cout << "player2 lefter" << std::endl;
    std::cout << player1->getPos().first << " " << player2->getPos().first << std::endl;
    player1->faceRight = false;
    player2->faceRight = true;
  }
  player1->handleInput();
  player2->handleInput();
};
void FightState::update(){ 
  player1->update();
  player2->update();

  std::cout << "p1 yVel: "<< player1->velocityY << std::endl;
  std::cout << "p1 yPos: "<< player1->getPos().second << std::endl;
};

void FightState::draw(){  
  currentScreen->draw();
  player1->draw();
  player2->draw();
};
