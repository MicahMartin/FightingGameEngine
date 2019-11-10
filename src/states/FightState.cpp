#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"

FightState::FightState(){ }

FightState::~FightState(){ }


void FightState::enter(){ 
  // init all fields
  currentScreen = new FightScreen();
  graphics->setCamera(&camera);
  camera.update(1600, 2300);
  
  player1 = new Character(std::make_pair(1600,0), 1);
  player2 = new Character(std::make_pair(2300,0), 2);
  player1->virtualController = inputManager->getVirtualController(0);
  player2->virtualController = inputManager->getVirtualController(1);
  
  charStateManager->registerCharacter(player1, 1);
  charStateManager->registerCharacter(player2, 2);

  player1->init();
  player1->virtualMachine.debugMode = true;
  player2->init();
}

void FightState::exit(){ 
  delete player1;
  delete player2;
  delete currentScreen;
}

void FightState::pause(){ }
void FightState::resume(){ }

void FightState::handleInput(){ 
  if (charStateManager->screenFrozen == false) {
    player1->handleInput();
    player2->handleInput();
  }
}

void FightState::update(){ 
  if(charStateManager->screenFrozen == false){
    player1->update();
    player2->update();

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

    for (auto p1Hitbox : player1->currentState->hitBoxes) {
      if(!p1Hitbox->disabled){
        for (auto p2HurtBox : player2->currentState->hurtBoxes) {
          if(!p2HurtBox->disabled){
            if (CollisionBox::checkAABB(*p1Hitbox, *p2HurtBox)) {
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
              player2->changeState(9);
              if(player2->comboCounter > 1){
                printf("player 2 been combo'd for %d hits\n", player2->comboCounter);
              }
            }
          }
        }
      }
    }


    // TODO: Refactor jesus
    // THIS IS THE UPDATE TURN STUFF
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

    // BOUND CHECKING
    // TODO: pls refactor one day
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

  } else {
    player1->currentState->handleCancels();
    player2->currentState->handleCancels();
    screenFreeze--;
    if(screenFreeze == 0){
      charStateManager->screenFrozen = false;
    }
  }
  camera.update(player1->getPos().first, player2->getPos().first);
}

void FightState::renderHealthBars(){
  int p1Hp = player1->health;
  float p1HpPercent = p1Hp / player1->maxHealth;
  int p2Hp = player2->health;
  float p2HpPercent = (float)p2Hp / (float)player2->maxHealth;
  SDL_Color green = {0, 255, 0, 0};
  SDL_Color red = {255, 0, 0, 0};

  // draw p1 healthbar
  renderHealthBar(100, 50, 500, 50, p1HpPercent, green, red);
  renderHealthBar(680, 50, 500, 50, p2HpPercent, green, red);
}

void FightState::renderHealthBar(int x, int y, int w, int h, float percent, SDL_Color fgColor, SDL_Color bgColor) {
  SDL_Color old;
  SDL_GetRenderDrawColor(graphics->getRenderer(), &old.r, &old.g, &old.g, &old.a);
  SDL_Rect bgrect = { x, y, w, h };
  SDL_SetRenderDrawColor(graphics->getRenderer(), bgColor.r, bgColor.g, bgColor.b, bgColor.a);
  SDL_RenderFillRect(graphics->getRenderer(), &bgrect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), fgColor.r, fgColor.g, fgColor.b, fgColor.a);
  int pw = (int)((float)w * percent);
  int px = x + (w - pw);
  SDL_Rect fgrect = { px, y, pw, h };
  SDL_RenderFillRect(graphics->getRenderer(), &fgrect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), old.r, old.g, old.b, old.a);
}

void FightState::draw(){  
  currentScreen->draw();
  renderHealthBars();
  player1->draw();
  player2->draw();
}
