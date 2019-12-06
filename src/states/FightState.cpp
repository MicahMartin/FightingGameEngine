#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"
#include "game_objects/Entity.h"

FightState::FightState(){ 
  printf("creating new fightState\n");
  stateName = "FightState";
}

FightState::~FightState(){ 
  printf("in fightState destructor\n");
}


void FightState::enter(){ 
  printf("entering fightState\n");
  // init all fields
  graphics->setCamera(&camera);
  camera.update(1700, 2200);
  player1 = new Character(std::make_pair(1700,0), 1);
  player2 = new Character(std::make_pair(2200,0), 2);
  player1->virtualController = inputManager->getVirtualController(0);
  player1->virtualController->initCommandCompiler();
  player2->virtualController = inputManager->getVirtualController(1);
  player2->virtualController->initCommandCompiler();

  player1->otherChar = player2;
  player2->otherChar = player1;
 
  charStateManager->registerCharacter(player1, 1);
  charStateManager->registerCharacter(player2, 2);

  player1->init();
  player2->init();
}

void FightState::exit(){ 
  printf("exiting fight state\n");
  delete player1;
  delete player2;
  delete this;
}

void FightState::pause(){ }
void FightState::resume(){ }

void FightState::handleInput(){ 
  updateFaceRight();
  checkHealth();
  if (!player1->inHitStop) {
    player1->handleInput();
  } else {
    // printf("p1HitStop:%d\n", player1->hitStop);
  }
  if (!player2->inHitStop) {
    player2->handleInput();
  } else {
    // printf("p2HitStop:%d\n", player2->hitStop);
  }

  for (auto &i : player1->entityList) {
    if(!i.inHitStop){
      i.handleInput();
    }
  }
  // printf("p1 entities handled input \n");
  for (auto &i : player2->entityList) {
    if(!i.inHitStop){
      i.handleInput();
    }
  }
  // printf("p2 entities handled input \n");
}

void FightState::update(){ 
  // printf("we made it into update!\n");
  stateTime++;
  if(player1->getPos().first - player1->width <= 0 || player1->getPos().first + player1->width >= 3840){
    player1->inCorner = true;
  } else {
    player1->inCorner = false;
  }

  if(player2->getPos().first - player2->width <= 0 || player2->getPos().first + player2->width >= 3840){
    player2->inCorner = true;
  } else {
    player2->inCorner = false;
  }


  if(player1->inHitStop && --player1->hitStop == 0){
    player1->inHitStop = false;
    // printf("p1 came out of hitstop\n");
  }
  if(player2->inHitStop && --player2->hitStop == 0){
    player2->inHitStop = false;
    // printf("p2 came out of hitstop\n");
  }
  // printf("we updated player hitstop!!\n");
  for (auto &i : player1->entityList) {
    if(i.inHitStop && --i.hitStop == 0){
      i.inHitStop = false;
      // printf("player:%d entity:%d came out of hitstop\n", 1, i.entityID);
    }
  }
  // printf("we updated player1 entity hitstop!!\n");
  for (auto &i : player2->entityList) {
    if(i.inHitStop && --i.hitStop == 0){
      i.inHitStop = false;
      // printf("player:%d entity:%d came out of hitstop\n", 2, i.entityID);
    }
  }
  // printf("we updated player2 entity hitstop!!\n");

  checkHitCollisions();
  // printf("got outta check collisions\n");

  if(!player1->inHitStop){
    player1->update();
  }
  if(!player2->inHitStop){
    player2->update();
  }
  if(player1->inHitStop){
    player1->currentState->handleCancels();
  }
  if(player2->inHitStop){
    player2->currentState->handleCancels();
  }
  // printf("did the hitstop checks\n");

  for (auto &i : player1->entityList) {
    if(!i.inHitStop){
      i.update();
      // printf("p1 entity:%d updating\n", i.entityID);
    }
  }
  for (auto &i : player2->entityList) {
    if(!i.inHitStop){
      i.update();
    }
  }
  for (auto &i : player1->entityList) {
    if(i.inHitStop){
      i.currentState->handleCancels();
      // printf("p1 entity:%d handling cancels\n", i.entityID);
    }
  }
  for (auto &i : player2->entityList) {
    if(i.inHitStop){
      i.currentState->handleCancels();
    }
  }
  // printf("updated the entities\n");

  checkPushCollisions();
  // printf("updated the collisions \n");
  checkBounds();
  // printf("bounsd were checked\n");
  camera.update(player1->getPos().first, player2->getPos().first);
  // printf("update gucci\n");
}

void FightState::draw(){  
  // printf("made it to draw\n");
  double screenDrawStart, screenDrawEnd,
         barDrawStart, barDrawEnd,
         p1DrawStart, p1DrawEnd,
         p2DrawStart, p2DrawEnd;
  screenDrawStart = SDL_GetTicks();
  currentScreen.draw();
  screenDrawEnd = SDL_GetTicks();
  // TODO: move renderHP into currentScreen
  barDrawStart = SDL_GetTicks();
  renderHealthBars();
  barDrawEnd = SDL_GetTicks();
  renderComboCount();
  renderInputHistory();

  if (player1->frameLastAttackConnected > player2->frameLastAttackConnected) {
    p2DrawStart = SDL_GetTicks();
    player2->draw();
    // printf("drew p2\n");
    for (auto &i : player2->entityList) {
      i.draw();
    }
    // printf("drew p2  entities\n");
    p2DrawEnd = SDL_GetTicks();

    p1DrawStart = SDL_GetTicks();
    player1->draw();
    // printf("drew p1\n");
    for (auto &i : player1->entityList) {
      i.draw();
    }
    // printf("drew p1 entities\n");
    p1DrawEnd = SDL_GetTicks();
  } else {
    p1DrawStart = SDL_GetTicks();
    player1->draw();
    // printf("drew p1\n");
    for (auto &i : player1->entityList) {
      i.draw();
    }
    // printf("drew p1 entities\n");
    p1DrawEnd = SDL_GetTicks();

    p2DrawStart = SDL_GetTicks();
    player2->draw();
    // printf("drew p2\n");
    for (auto &i : player2->entityList) {
      i.draw();
    }
    // printf("drew p1 entities\n");
    p2DrawEnd = SDL_GetTicks();
  }
   double screenDraw = screenDrawEnd - screenDrawStart;
   double barDraw = barDrawEnd - barDrawStart;
   double p1Draw = p1DrawEnd - p1DrawStart;
   double p2Draw = p2DrawEnd - p2DrawStart;
   if (screenDraw > 0) {
     // printf("screenDraw %f\n", screenDraw);
   }
   if (barDraw > 0) {
     // printf("barDraw %f\n", barDraw);
   }
   if(p1Draw > 0) {
     // printf("p1Draw %f\n", p1Draw);
   }
   if(p2Draw > 0){
     // printf("p2Draw %f\n", p2Draw);
   }
   // printf("good draw?!?!\n");
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
            // find how deeply intersected they are
            bool p1Lefter = p1Pos.first < p2Pos.first;

            if(p1Lefter){
              int p1RightEdge = p1PushBox->positionX + p1PushBox->width;
              int p2LeftEdge = p2PushBox->positionX;
              int depth = p1RightEdge - p2LeftEdge;

              // account for over bound 
              if ((p2Pos.first+player2->width) + (depth/2) > 3840) {
                int remainder = 3840 - (p2Pos.first + (depth/2));
                player2->setXPos(3840-player2->width);
                player1->setX(-depth);
              } else if ((p1Pos.first - player1->width) - (depth/2) < 0){
                int remainder = p1Pos.first + (depth/2);
                player1->setXPos(0+player1->width);
                player2->setX(depth);
              } else {
                player2->setX(depth/2);
                player1->setX(-depth/2);
              }
            } else {
              int p2RightEdge = p2PushBox->positionX + p2PushBox->width;
              int p1LeftEdge = p1PushBox->positionX;
              int depth = p2RightEdge - p1LeftEdge;

              // account for over bound 
              if ((p1Pos.first+player1->width) + (depth/2) > 3840) {
                int remainder = 3840 - (p1Pos.first + (depth/2));
                player1->setXPos(3840+player1->width);
                player2->setX(-depth);
              } else if ((p2Pos.first - player2->width) - (depth/2) < 0){
                int remainder = p2Pos.first + (depth/2);
                player2->setXPos(0+player2->width);
                player1->setX(depth);
              } else {
                player2->setX(-depth/2);
                player1->setX(depth/2);
              }
            }
            player1->updateCollisionBoxPositions();
            player2->updateCollisionBoxPositions();
          }
        }
      }
    }
  }
}

void FightState::checkHitCollisions(){
  if (!player1->currentState->hitboxesDisabled) {
    for (auto p1Hitbox : player1->currentState->hitBoxes) {
      if(!p1Hitbox->disabled){
        for (auto p2HurtBox : player2->currentState->hurtBoxes) {
          if(!p2HurtBox->disabled){
            if (CollisionBox::checkAABB(*p1Hitbox, *p2HurtBox)) {
              printf("hitbox collision detected\n");
              player1->inHitStop = true;
              player2->inHitStop = true;
              player1->hitStop = p1Hitbox->hitstop;
              player2->hitStop = p1Hitbox->hitstop;

              player1->frameLastAttackConnected = gameTime; 
              // TODO: Hitbox group IDs
              player1->currentState->hitboxesDisabled = true;

              if (player2->inCorner) {
                player1->pushTime = p1Hitbox->pushTime;
                player1->_negVelSetX(p1Hitbox->pushback);
              } else {
                player2->pushTime = p1Hitbox->pushTime;
                player2->_negVelSetX(p1Hitbox->pushback);
              }

              if(checkBlock(p1Hitbox->blockType, player2) && ((player2->currentState->stateNum == 28 || player2->currentState->stateNum == 29) || player2->control)){
                player2->blockstun = p1Hitbox->blockstun;
                player2->control = 0;
                if (player2->_getYPos() > 0) {
                  player2->changeState(29);
                } else {
                  switch (p1Hitbox->blockType) {
                    case 1:
                      if (player2->_getInput(1)) {
                        player2->changeState(29);
                      } else {
                        player2->changeState(28);
                      }
                      break;
                    case 2:
                      player2->changeState(29);
                      break;
                    case 3:
                      player2->changeState(28);
                      break;
                    // should throw error here
                    default: break;
                  }
                }
                printf("ohh u got the blocksies?\n");
              } else {
                printf("ya wasnt blockin kid\n");
                player2->control = 0;
                player2->health -= p1Hitbox->damage;
                player2->hitstun = p1Hitbox->hitstun;
                player2->comboCounter++;

                if(p1Hitbox->canTrip || player2->_getYPos() > 0 || player2->currentState->stateNum == 24){
                  player2->changeState(24);
                } else {
                  player2->changeState(9);
                }
              }
            }
          }
        }
      }
    }
  }
  checkEntityHitCollisions();
}

void FightState::checkEntityHitCollisions(){
  for (auto &i : player1->entityList) {
    if (!i.currentState->hitboxesDisabled) {
      for (auto p1Hitbox : i.currentState->hitBoxes) {
        if(!p1Hitbox->disabled){
          for (auto p2HurtBox : player2->currentState->hurtBoxes) {
            if(!p2HurtBox->disabled){
              if (CollisionBox::checkAABB(*p1Hitbox, *p2HurtBox)) {
                printf("entity hitbox collision detected\n");
                bool entityFaceRight = i.faceRight;
                i.inHitStop = true;
                i.hitStop = p1Hitbox->hitstop;

                player2->inHitStop = true;
                player2->hitStop = p1Hitbox->hitstop;

                player1->frameLastAttackConnected = gameTime; 
                // TODO: Hitbox group IDs
                i.currentState->hitboxesDisabled = true;

                player2->pushTime = p1Hitbox->pushTime;
                if(player2->faceRight == entityFaceRight){
                  player2->_velSetX(p1Hitbox->pushback);
                } else {
                  player2->_negVelSetX(p1Hitbox->pushback);
                }

                if(checkBlock(p1Hitbox->blockType, player2) && ((player2->currentState->stateNum == 28 || player2->currentState->stateNum == 29) || player2->control)){
                  player2->blockstun = p1Hitbox->blockstun;
                  player2->control = 0;
                  if (player2->_getYPos() > 0) {
                    // TODO: air blocking state
                    player2->changeState(29);
                  } else {
                    switch (p1Hitbox->blockType) {
                      case 1:
                        if (player2->_getInput(1)) {
                          player2->changeState(29);
                        } else {
                          player2->changeState(28);
                        }
                        break;
                      case 2:
                        player2->changeState(29);
                        break;
                      case 3:
                        player2->changeState(28);
                        break;
                      // should throw error here
                      default: break;
                    }
                  }
                  printf("ohh u got the blocksies?\n");
                } else {
                  printf("ya wasnt blockin kid\n");
                  player2->control = 0;
                  player2->health -= p1Hitbox->damage;
                  player2->hitstun = p1Hitbox->hitstun;
                  player2->comboCounter++;

                  if(p1Hitbox->canTrip || player2->_getYPos() > 0 || player2->currentState->stateNum == 24){
                    player2->changeState(24);
                  } else {
                    player2->changeState(9);
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

bool FightState::checkBlock(int blockType, Character* player){
  bool isHoldingDownBack = player->_getInput(1);
  bool isHoldingBack = player->_getInput(4);
  printf("player2 is holding downback? %d, what about back %d\n", isHoldingDownBack, isHoldingBack);
  // I know, enum
  switch (blockType) {
    // mid
    case 1:
      if(isHoldingDownBack || isHoldingBack)
        return true;
      break;
    // low
    case 2:
      if(isHoldingDownBack)
        return true;
      break;
    case 3:
      // high
      if(isHoldingBack)
        return true;
      break;
    default:
      return true;
  }

  return false;
}

void FightState::checkBounds(){
  if(player1->getPos().first - player1->width <= 0) {
    player1->setXPos(0+player1->width);
    player1->updateCollisionBoxPositions();
  }
  if (player1->getPos().first - player1->width < camera.lowerBound) {
    printf("why am I outside the camera on the left?\n");
    player1->setXPos(camera.lowerBound+player1->width);
    player1->updateCollisionBoxPositions();
  }

  if(player1->getPos().first + player1->width >= 3840) {
    player1->setXPos(3840 - player1->width);
    player1->updateCollisionBoxPositions();
  }
  if (player1->getPos().first + player1->width > camera.upperBound) {
    printf("why am I outside the camera on the right?\n");
    player1->setXPos(camera.upperBound - player1->width);
    player1->updateCollisionBoxPositions();
  }

  if(player2->getPos().first - player2->width <= 0) {
    player2->setXPos(0 + player2->width);
    player2->updateCollisionBoxPositions();
  }
  if (player2->getPos().first - player2->width < camera.lowerBound) {
    player2->setXPos(camera.lowerBound + player2->width);
    player2->updateCollisionBoxPositions();
  }

  if(player2->getPos().first + player2->width >= 3840) {
    player2->setXPos(3840 - player2->width);
    player2->updateCollisionBoxPositions();
  }
  if (player2->getPos().first + player2->width > camera.upperBound) {
    player2->setXPos(camera.upperBound - player2->width);
    player2->updateCollisionBoxPositions();
  }
}

void FightState::checkHealth(){
  if (player1->health <= 0 || player2->health <= 0) {
    player2->health = 100;
    player1->health = 100;
  }
}

void FightState::updateFaceRight(){
  if(player1->getPos().first < player2->getPos().first){
    player1->inputFaceRight = true;
    player2->inputFaceRight = false;

    if(!player1->currentState->checkFlag(NO_TURN)){
      player1->faceRight = true;
    }
    if(!player2->currentState->checkFlag(NO_TURN)){
      player2->faceRight = false;
    }
  } else {
    player1->inputFaceRight = false;
    player2->inputFaceRight = true;

    if(!player1->currentState->checkFlag(NO_TURN)){
      player1->faceRight = false;
    }
    if(!player2->currentState->checkFlag(NO_TURN)){
      player2->faceRight = true;
    }
  }
  for (auto &i : player1->entityList) {
    if (i.active && i.updateFacing) {
      if(i.getPos().first < player2->getPos().first){
        i.inputFaceRight = true;
        if(!i.currentState->checkFlag(NO_TURN)){
          i.faceRight = true;
        }
      } else {
        i.inputFaceRight = false;
        if(!i.currentState->checkFlag(NO_TURN)){
          i.faceRight = false;
        }
      }
    }
  }
  for (auto &i : player2->entityList) {
    if (i.active && i.updateFacing) {
      if(i.getPos().first < player1->getPos().first){
        i.inputFaceRight = true;
        if(!i.currentState->checkFlag(NO_TURN)){
          i.faceRight = true;
        }
      } else {
        i.inputFaceRight = false;
        if(!i.currentState->checkFlag(NO_TURN)){
          i.faceRight = false;
        }
      }
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
  int p2ComboCount = player2->comboCounter;
  if(p2ComboCount > 0){
    currentScreen.renderComboCount(true, p2ComboCount);
  }
  if(p1ComboCount > 0){
    currentScreen.renderComboCount(false, p1ComboCount);
  }
}

void FightState::renderInputHistory(){
  // printf("p1InputEventList size %ld\n", player1->virtualController->inputEventList.size());
  currentScreen.renderInputHistory(true, player1->virtualController->inputEventList);
  currentScreen.renderInputHistory(false, player2->virtualController->inputEventList);
}
