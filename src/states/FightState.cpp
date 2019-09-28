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

  player1->init();
  player2->init();

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
  player1->handleInput();
  player2->handleInput();
};

void FightState::update(){ 
  player1->update();
  player2->update();

  // get the collision box(s) for the current state
  std::vector<CollisionBox> p1PushBoxes = player1->currentState->pushBoxes;
  std::vector<CollisionBox> p2PushBoxes = player2->currentState->pushBoxes;
  std::pair<int, int> p1Pos = player1->getPos();
  std::pair<int, int> p2Pos = player2->getPos();

  // I know
  for (auto p1PushBox : p1PushBoxes) {
    for (auto p2PushBox : p2PushBoxes) {
      if (CollisionBox::checkAABB(p1PushBox, p2PushBox)) {
        // if p1 is in the air and p2 is not
        if (p1Pos.second < 0 && p2Pos.second >= 0) {
          // find how deeply intersected they are
          printf("p1 in the air annd p2 grounded\n");
          bool p1Lefter = p1Pos.first < p2Pos.first;

          if(p1Lefter){
            std::cout << "p1lefter" << std::endl;
            int p1RightEdge = p1PushBox.positionX + p1PushBox.width;
            int p2LeftEdge = p2PushBox.positionX;

            if(p2LeftEdge < p1RightEdge){
              player2->setX(p1RightEdge - p2LeftEdge);
            }
          } else {
            int p1LeftEdge = p1PushBox.positionX;
            int p2RightEdge = p2PushBox.positionX + p2PushBox.width;

            if(p2RightEdge > p1LeftEdge){
              player2->setX(p1LeftEdge - p2RightEdge);
            }
          }
        } else if (p2Pos.second < 0 && p1Pos.second >= 0) {
          printf("p2 in the air annd p1 grounded\n");
        } else {
          int p1Vel = player1->velocityX;
          int p2Vel = player2->velocityX;
          player1->setX(p2Vel);
          player2->setX(p1Vel);
        }
      }
    }
  }

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

  if(player1->getPos().first < 0) {
    player1->setXPos(0);
  }

  if(player1->getPos().first > 1280) {
    player1->setXPos(1280);
  }

  if(player2->getPos().first < 0) {
    player2->setXPos(0);
  }

  if(player2->getPos().first > 1280) {
    player2->setXPos(1280);
  }

  // check hitboxes
  std::vector<CollisionBox> p1Hitboxes = player1->currentState->hitBoxes;
  std::vector<CollisionBox> p1HurtBoxes = player1->currentState->hurtBoxes;

  std::vector<CollisionBox> p2Hitboxes = player2->currentState->hitBoxes;
  std::vector<CollisionBox> p2HurtBoxes = player2->currentState->hurtBoxes;

  for (auto p1Hitbox : p1Hitboxes) {
    for (auto p2HurtBox : p2HurtBoxes) {
      if (CollisionBox::checkAABB(p1Hitbox, p2HurtBox)) {
        printf("holy fuck lets go\n");
      }
    }
  }
};

void FightState::draw(){  
  currentScreen->draw();
  player1->draw();
  player2->draw();
};
