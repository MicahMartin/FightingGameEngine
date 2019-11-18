#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"

FightState::FightState(){ 
  stateName = "FightState";
}

FightState::~FightState(){ }


void FightState::enter(){ 
  // init all fields
  graphics->setCamera(&camera);
  camera.update(1700, 2200);
  
  player1 = new Character(std::make_pair(1700,0), 1);
  player2 = new Character(std::make_pair(2200,0), 2);
  player1->virtualController = inputManager->getVirtualController(0);
  player2->virtualController = inputManager->getVirtualController(1);
  
  charStateManager->registerCharacter(player1, 1);
  charStateManager->registerCharacter(player2, 2);

  player1->init();
  player2->init();
}

void FightState::exit(){ 
  delete player1;
  delete player2;
}

void FightState::pause(){ }
void FightState::resume(){ }

void FightState::handleInput(){ 
  updateFaceRight();
  checkHealth();
  if (charStateManager->screenFrozen == false) {
    player1->handleInput();
    player2->handleInput();
  }
}

void FightState::update(){ 
  stateTime++;
  if (stateTime > 120) {
    everythingCompiled = true;
  }
  if(charStateManager->screenFrozen == false){
    checkPushCollisions();
    checkHitCollisions();
    checkBounds();
    player1->update();
    player2->update();
  } else {
    if(--screenFreeze == 0){
      charStateManager->screenFrozen = false;
    }
    player1->currentState->handleCancels();
    player2->currentState->handleCancels();
  }
  camera.update(player1->getPos().first, player2->getPos().first);
}

void FightState::draw(){  
  double screenDrawStart, screenDrawEnd,
         barDrawStart, barDrawEnd,
         p1DrawStart, p1DrawEnd,
         p2DrawStart, p2DrawEnd;
  screenDrawStart = SDL_GetTicks();
  // currentScreen.draw();
  screenDrawEnd = SDL_GetTicks();
  // TODO: move renderHP into currentScreen
  barDrawStart = SDL_GetTicks();
  // renderHealthBars();
  barDrawEnd = SDL_GetTicks();
  if (player1->frameLastAttackConnected > player2->frameLastAttackConnected) {
    p2DrawStart = SDL_GetTicks();
    player2->draw();
    p2DrawEnd = SDL_GetTicks();

    p1DrawStart = SDL_GetTicks();
    player1->draw();
    p1DrawEnd = SDL_GetTicks();
  } else {
    p1DrawStart = SDL_GetTicks();
    player1->draw();
    p1DrawEnd = SDL_GetTicks();

    p2DrawStart = SDL_GetTicks();
    player2->draw();
    p2DrawEnd = SDL_GetTicks();
  }
   double screenDraw = screenDrawEnd - screenDrawStart;
   double barDraw = barDrawEnd - barDrawStart;
   double p1Draw = p1DrawEnd - p1DrawStart;
   double p2Draw = p2DrawEnd - p2DrawStart;
   if (screenDraw > 0) {
     printf("screenDraw %f\n", screenDraw);
   }
   if (barDraw > 0) {
     printf("barDraw %f\n", barDraw);
   }
   if(p1Draw > 0) {
     printf("p1Draw %f\n", p1Draw);
   }
   if(p2Draw > 0){
     printf("p2Draw %f\n", p2Draw);
   }
}

void FightState::checkPushCollisions(){
  // get the collision box(s) for the current state
  std::pair<int, int> p1Pos = player1->getPos();
  std::pair<int, int> p2Pos = player2->getPos();

  for (auto p1PushBox : player1->currentState->pushBoxes) {
    if(!p1PushBox->disabled){
      for (auto p2PushBox : player2->currentState->pushBoxes) {
        if(!p2PushBox->disabled){
          if (CollisionBox::checkAABB(*p1PushBox, *p2PushBox)) {
            // printf("pushbox collision detected\n");
            // if p1 is in the air and p2 is not
            if (p1Pos.second < 0 && p2Pos.second >= 0) {
              // find how deeply intersected they are
              bool p1Lefter = p1Pos.first < p2Pos.first;

              if(p1Lefter){
                std::cout << "p1lefter" << std::endl;
                int p1RightEdge = p1PushBox->positionX + p1PushBox->width;
                int p2LeftEdge = p2PushBox->positionX;

                if(p2LeftEdge < p1RightEdge){
                  player2->setX(p1RightEdge - p2LeftEdge);
                }
              } else {
                int p1LeftEdge = p1PushBox->positionX;
                int p2RightEdge = p2PushBox->positionX + p2PushBox->width;

                if(p2RightEdge > p1LeftEdge){
                  player2->setX(p1LeftEdge - p2RightEdge);
                }
              }
            } else if (p2Pos.second < 0 && p1Pos.second >= 0) {
              printf("p2 in the air annd p1 grounded\n");
            } else {
              int p1Vel = player1->velocityX;
              int p2Vel = player2->velocityX;
              if(p1Pos.first == 0 || p1Pos.first == 3840){
                player1->setX(p2Vel);
                player2->setX(-p2Vel);
              } else if (p2Pos.first == 0 || p2Pos.first == 3840){
                player1->setX(-p1Vel);
                player2->setX(p1Vel);
              } else {
                player1->setX(p2Vel);
                player2->setX(p1Vel);
              }
            }
          }
        }
      }
    }
  }
}

void FightState::checkHitCollisions(){
  for (auto p1Hitbox : player1->currentState->hitBoxes) {
    if(!p1Hitbox->disabled){
      for (auto p2HurtBox : player2->currentState->hurtBoxes) {
        if(!p2HurtBox->disabled){
          if (CollisionBox::checkAABB(*p1Hitbox, *p2HurtBox)) {
            player1->frameLastAttackConnected = gameTime; 
            printf("hitbox collision detected\n");
            // TODO: Run hitscript
            charStateManager->screenFrozen = true;
            p1Hitbox->disabled = true;
            screenFreeze = p1Hitbox->hitstop;

            player2->control = 0;
            player2->health -= p1Hitbox->damage;
            player2->hitstun = p1Hitbox->hitstun;
            player2->_negVelSetX(p1Hitbox->pushback);
            player2->comboCounter++;
            if(p1Hitbox->canTrip){
              player2->changeState(24);
            } else {
              player2->changeState(9);
            }
            if(player2->comboCounter > 1){
              printf("player 2 been combo'd for %d hits\n", player2->comboCounter);
            }
          }
        }
      }
    }
  }
}

void FightState::checkBounds(){
  if(player1->getPos().first < 0) {
    player1->setXPos(0);
  }
  if (player1->getPos().first < camera.lowerBound) {
    printf("why am I outside the camera on the left?\n");
    player1->setXPos(camera.lowerBound);
  }

  if(player1->getPos().first > 3840) {
    player1->setXPos(3840);
  }
  if (player1->getPos().first > camera.upperBound) {
    printf("why am I outside the camera on the right?\n");
    player1->setXPos(camera.upperBound);
  }

  if(player2->getPos().first < 0) {
    player2->setXPos(0);
  }
  if (player2->getPos().first < camera.lowerBound) {
    player2->setXPos(camera.lowerBound);
  }

  if(player2->getPos().first > 3840) {
    player2->setXPos(3840);
  }
  if (player2->getPos().first > camera.upperBound) {
    player2->setXPos(camera.upperBound);
  }
}

void FightState::checkHealth(){
  if (player1->health <= 0) {
    player1->health = 100;
  }
  if (player2->health <= 0) {
    player2->health = 100;
  }
}

void FightState::updateFaceRight(){
  if(player1->getPos().first <= player2->getPos().first){
    if(!player1->currentState->checkFlag(NO_TURN)){
      player1->faceRight = true;
    }
    if(!player2->currentState->checkFlag(NO_TURN)){
      player2->faceRight = false;
    }
  } else {
    if(!player1->currentState->checkFlag(NO_TURN)){
      player1->faceRight = false;
    }
    if(!player2->currentState->checkFlag(NO_TURN)){
      player2->faceRight = true;
    }
  }
}

void FightState::renderHealthBars(){
  int p1Hp = player1->health;
  float p1HpPercent = p1Hp / player1->maxHealth;
  int p2Hp = player2->health;
  float p2HpPercent = (float)p2Hp / (float)player2->maxHealth;

  SDL_Color green = {0, 255, 0, 0};
  SDL_Color red = {255, 0, 0, 0};
  // draw p1 healthbar
  currentScreen.renderHealthBar(100, 50, 500, 50, p1HpPercent, green, red);
  currentScreen.renderHealthBar(680, 50, 500, 50, p2HpPercent, green, red);
}

void FightState::renderComboCount(){
  int p1ComboCount = player1->comboCounter;
  int p2ComboCount = player1->comboCounter;
  if(p1ComboCount > 0){
    currentScreen.renderComboCount(true, p1ComboCount);
  }
  if(p2ComboCount > 0){
    currentScreen.renderComboCount(false, p2ComboCount);
  }
}

void FightState::renderInputHistory(){
}
