#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"
#include "game_objects/Entity.h"

FightState::FightState(){ 
  printf("creating new fightState\n");
  stateName = "FightState";
  bgMusic = Mix_LoadMUS("../data/audio/fightingTheme.mp3");
  Mix_VolumeMusic(32);
  for (int i = 1; i < 8; ++i) {
    Mix_Volume(i, 32);
  }
  yawl_ready = Mix_LoadWAV("../data/audio/uiSounds/yawl_ready.mp3");
  round1Sound = Mix_LoadWAV("../data/audio/uiSounds/round_1.mp3");
  round2Sound = Mix_LoadWAV("../data/audio/uiSounds/round_2.mp3");
  finalRoundSound = Mix_LoadWAV("../data/audio/uiSounds/final_round.mp3");
  fightSound = Mix_LoadWAV("../data/audio/uiSounds/fight.mp3");
  koSound = Mix_LoadWAV("../data/audio/uiSounds/KO.mp3");
  p1WinSound = Mix_LoadWAV("../data/audio/uiSounds/player_1_wins.mp3");
  p2WinSound = Mix_LoadWAV("../data/audio/uiSounds/player_2_win.mp3");
  countah = Mix_LoadWAV("../data/audio/uiSounds/countah.mp3");
  instantBlock = Mix_LoadWAV("../data/audio/uiSounds/instantBlock.mp3");

  if(bgMusic == NULL) {
    printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
  }
}

FightState::~FightState(){ 
  printf("in fightState destructor\n");
}

void FightState::enter(){ 
  printf("entering fightState\n");

  player1 = new Character(std::make_pair(1700,0), 1);
  player1->virtualController = inputManager->getVirtualController(0);
  player1->virtualController->initCommandCompiler();
  player1->soundChannel = 2;
  player1->gravityVal = 1;
  charStateManager->registerCharacter(player1, 1);

  player2 = new Character(std::make_pair(2200,0), 2);
  player2->virtualController = inputManager->getVirtualController(1);
  player2->virtualController->initCommandCompiler();
  player2->soundChannel = 5;
  player2->gravityVal = 1;
  charStateManager->registerCharacter(player2, 2);


  player1->otherChar = player2;
  player1->init();
  player2->otherChar = player1;
  player2->init();
  player1->control = 0;
  player2->control = 0;

  graphics->setCamera(&camera);
  camera.update(1700, 2200);
  Mix_PlayMusic(bgMusic, -1);
  roundStartCounter = 210;
  roundStart = true;

  matchIntroPopup.loadDataFile("../data/images/UI/pop_up/match_intro/data.json");
  matchIntroPopup.setPlayLength(60);
  matchIntroPopup.setX(camera.middle);

  round1.loadDataFile("../data/images/UI/pop_up/round_1/data.json");
  round1.setPlayLength(60);
  round1.setX(camera.middle);

  round2Popup.loadDataFile("../data/images/UI/pop_up/round_2/data.json");
  round2Popup.setPlayLength(60);
  round2Popup.setX(camera.middle);

  finalRoundPopup.loadDataFile("../data/images/UI/pop_up/round_3/data.json");
  finalRoundPopup.setPlayLength(60);
  finalRoundPopup.setX(camera.middle);

  fightPopup.loadDataFile("../data/images/UI/pop_up/fight/data.json");
  fightPopup.setPlayLength(60);
  fightPopup.setX(camera.middle);

  knockoutPopup.loadDataFile("../data/images/UI/pop_up/knockout/data.json");
  knockoutPopup.setPlayLength(60);

  p1WinPopup.loadDataFile("../data/images/UI/pop_up/player_1_win/data.json");
  p1WinPopup.setPlayLength(120);

  p2WinPopup.loadDataFile("../data/images/UI/pop_up/player_2_win/data.json");
  p2WinPopup.setPlayLength(120);

  p1CounterHit.loadDataFile("../data/images/UI/pop_up/counter/data.json");
  p1CounterHit.setPlayLength(30);
  p2CounterHit.loadDataFile("../data/images/UI/pop_up/counter/data.json");
  p2CounterHit.setPlayLength(30);

  Mix_Volume(0, 32);
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
  if(!slowMode){
    if (roundStartCounter > 0) {
      // printf("roundStarTCounter!:%d\n", roundStartCounter);
      if (--roundStartCounter == 0) {
        player1->control = 1;
        player2->control = 1;
        roundStart = false;
      }
      if (roundStartCounter == 200) {
        matchIntroPopup.setStateTime(0);
        matchIntroPopup.setActive(true);
        Mix_PlayChannel(0, yawl_ready, 0);
      }
      if (roundStartCounter == 130) {
        switch (currentRound) {
          case 0:
            round1.setStateTime(0);
            round1.setActive(true);
            Mix_PlayChannel(0, round1Sound, 0);
          break;
          case 1:
            round2Popup.setStateTime(0);
            round2Popup.setActive(true);
            Mix_PlayChannel(0, round2Sound, 0);
          break;
          case 2:
            finalRoundPopup.setStateTime(0);
            finalRoundPopup.setActive(true);
            Mix_PlayChannel(0, finalRoundSound, 0);
          break;
          default:
          break;
        }
      }

      if (roundStartCounter == 60) {
        fightPopup.setStateTime(0);
        fightPopup.setActive(true);
        Mix_PlayChannel(0, fightSound, 0);
      }
    }
    checkCorner(player1);
    checkCorner(player2);
    updateFaceRight();
    checkHitstop(player1);
    checkHitstop(player2);

    checkEntityHitstop(player1);
    checkEntityHitstop(player2);

    if (!player1->inHitStop) {
      player1->handleInput();
    }
    if (!player2->inHitStop) {
      player2->handleInput();
    }

    for (auto &i : player1->entityList) {
      if(!i.inHitStop){
        i.handleInput();
      }
    }
    for (auto &i : player2->entityList) {
      if(!i.inHitStop){
        i.handleInput();
      }
    }
    // check for throw techs
    if (player1->currentState->checkFlag(TECHABLE)) {
      if (player1->_checkCommand(5)) {
        int techState = player1->currentState->techState;
        player1->changeState(techState);
        player2->changeState(techState);
      }
    }
    if (player2->currentState->checkFlag(TECHABLE)) {
      if (player2->_checkCommand(5)) {
        int techState = player2->currentState->techState;
        player1->changeState(techState);
        player2->changeState(techState);
      }
    }
  }
  player1->currentState->handleCancels();
  player2->currentState->handleCancels();
  for (auto &i : player1->entityList) {
    i.currentState->handleCancels();
  }
  for (auto &i : player2->entityList) {
    i.currentState->handleCancels();
  }
  
  checkProximityAgainst(player1, player2);
  checkProximityAgainst(player2, player1);

  checkThrowCollisions();
  checkProjectileCollisions(player1, player2);
  checkHitCollisions();
  checkBounds();
  updateFaceRight();
  checkCorner(player1);
  checkCorner(player2);
}

void FightState::update(){
  // printf("we made it into update!\n");
  if(!slowMode){

    if(!player1->inHitStop){
      player1->update();
    }

    if(!player2->inHitStop){
      player2->update();
    }

    for (auto &i : player1->entityList) {
      if(!i.inHitStop){
        i.update();
      }
    }
    for (auto &i : player2->entityList) {
      if(!i.inHitStop){
        i.update();
      }
    }
  }
  checkBounds();
  updateFaceRight();
  checkCorner(player1);
  checkCorner(player2);

  int highest = player1->_getYPos() > player2->_getYPos() ? player1->_getYPos() : player2->_getYPos();
  camera.update(player1->getPos().first, player2->getPos().first);
  if(highest > (graphics->getWindowHeight()/2)){
    camera.cameraRect.y = highest - (graphics->getWindowHeight() / 2);
  } else {
    camera.cameraRect.y = 0;
  }
  checkPushCollisions();
  checkBounds();
  checkHealth();

  if (slowMode) {
    if(slowDownCounter++ == 70){
      slowDownCounter = 0;
      slowMode = false;
      roundEnd = true;

      if (roundWinner == 1) {
        p1WinPopup.setX(camera.middle);
        p1WinPopup.setY(camera.cameraRect.y);
        p1WinPopup.setStateTime(0);
        p1WinPopup.setActive(true);
        Mix_PlayChannel(0, p1WinSound, 0);
      } else if (roundWinner == 2) {
        p2WinPopup.setX(camera.middle);
        p2WinPopup.setY(camera.cameraRect.y);
        p2WinPopup.setStateTime(0);
        p2WinPopup.setActive(true);
        Mix_PlayChannel(0, p2WinSound, 0);
      }
      roundWinner = 0;
    }
  }

  if (matchIntroPopup.getActive()) {
    matchIntroPopup.update();
  }
  if (round1.getActive()) {
    round1.update();
  }
  if (round2Popup.getActive()) {
    round2Popup.update();
  }
  if (finalRoundPopup.getActive()) {
    finalRoundPopup.update();
  }
  if (fightPopup.getActive()) {
    fightPopup.update();
  }
  if (knockoutPopup.getActive()) {
    knockoutPopup.setX(camera.middle);
    knockoutPopup.setY(camera.cameraRect.y);
    knockoutPopup.update();
  }
  if (p1WinPopup.getActive()) {
    p1WinPopup.setX(camera.middle);
    p1WinPopup.setY(camera.cameraRect.y);
    p1WinPopup.update();
  }
  if (p2WinPopup.getActive()) {
    p2WinPopup.setX(camera.middle);
    p2WinPopup.setY(camera.cameraRect.y);
    p2WinPopup.update();
  }
  if (p1CounterHit.getActive()) {
    p1CounterHit.setX(camera.lowerBound);
    p1CounterHit.setY(camera.cameraRect.y);
    p1CounterHit.update();
  }
  if (p2CounterHit.getActive()) {
    p2CounterHit.setX(camera.upperBound - 350);
    p2CounterHit.setY(camera.cameraRect.y);
    p2CounterHit.update();
  }

  // TODO: Container of base class pointer to all this shit
  for (auto &i : player1->visualEffects) {
    i.second.update();
  }
  for (auto &i : player2->visualEffects) {
    i.second.update();
  }
  for (auto &i : player1->guardSparks) {
    i.second.update();
  }
  for (auto &i : player2->guardSparks) {
    i.second.update();
  }
  for (auto &i : player1->hitSparks) {
    i.second.update();
  }
  for (auto &i : player2->hitSparks) {
    i.second.update();
  }

  for (auto &e : player1->entityList) {
    for (auto &i : e.visualEffects) {
      i.second.update();
    }
    for (auto &i : e.hitSparks) {
      i.second.update();
    }
  }
  for (auto &e : player2->entityList) {
    for (auto &i : e.visualEffects) {
      i.second.update();
    }
    for (auto &i : e.hitSparks) {
      i.second.update();
    }
  }
}

void FightState::draw() {
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
  currentScreen.renderWins(p1RoundsWon, p2RoundsWon);
  if (p1CounterHit.getActive()) {
    p1CounterHit.draw();
  }
  if (p2CounterHit.getActive()) {
    p2CounterHit.draw();
  }

  if (player1->frameLastAttackConnected > player2->frameLastAttackConnected) {
    p2DrawStart = SDL_GetTicks();
    player2->draw();
    for (auto &i : player2->entityList) {
      i.draw();
      for (auto &e : i.visualEffects) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
      for (auto &e : i.hitSparks) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
    }
    for (auto &i : player2->visualEffects) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    for (auto &i : player2->guardSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    for (auto &i : player2->hitSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    p2DrawEnd = SDL_GetTicks();

    p1DrawStart = SDL_GetTicks();
    player1->draw();
    for (auto &i : player1->entityList) {
      i.draw();
      for (auto &e : i.visualEffects) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
      for (auto &e : i.hitSparks) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
    }
    for (auto &i : player1->visualEffects) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player1->faceRight);
      }
    }
    for (auto &i : player1->guardSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    for (auto &i : player1->hitSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    p1DrawEnd = SDL_GetTicks();
  } else {
    p1DrawStart = SDL_GetTicks();
    player1->draw();
    for (auto &i : player1->entityList) {
      i.draw();
      for (auto &e : i.visualEffects) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
      for (auto &e : i.hitSparks) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
    }
    for (auto &i : player1->visualEffects) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player1->faceRight);
      }
    }
    for (auto &i : player1->guardSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player1->faceRight);
      }
    }
    for (auto &i : player1->hitSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player1->faceRight);
      }
    }
    // printf("drew p1 entities\n");
    p1DrawEnd = SDL_GetTicks();

    p2DrawStart = SDL_GetTicks();
    player2->draw();
    // printf("drew p2\n");
    for (auto &i : player2->entityList) {
      i.draw();
      for (auto &e : i.visualEffects) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
      for (auto &e : i.hitSparks) {
        if (e.second.getActive()) {
          e.second.draw(i.faceRight);
        }
      }
    }
    for (auto &i : player2->visualEffects) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    for (auto &i : player2->guardSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }
    for (auto &i : player2->hitSparks) {
      VisualEffect& visFX = i.second;
      if (visFX.getActive()) {
        visFX.draw(player2->faceRight);
      }
    }

    // printf("drew p1 entities\n");
    p2DrawEnd = SDL_GetTicks();
  }
  double screenDraw = screenDrawEnd - screenDrawStart;
  double barDraw = barDrawEnd - barDrawStart;
  double p1Draw = p1DrawEnd - p1DrawStart;
  double p2Draw = p2DrawEnd - p2DrawStart;

  // pls refactor
  if (matchIntroPopup.getActive()) {
    matchIntroPopup.draw();
  }
  if (round1.getActive()) {
    round1.draw();
  }
  if (round2Popup.getActive()) {
    round2Popup.draw();
  }
  if (finalRoundPopup.getActive()) {
    finalRoundPopup.draw();
  }
  if (fightPopup.getActive()) {
    fightPopup.draw();
  }
  if (knockoutPopup.getActive()) {
    knockoutPopup.draw();
  }
  if (p1WinPopup.getActive()) {
    p1WinPopup.draw();
  }
  if (p2WinPopup.getActive()) {
    p2WinPopup.draw();
  }

  for (auto& i : player1->soundsEffects) {
    SoundObj& soundEffect = i.second;
    if (soundEffect.active) {
      Mix_PlayChannel(soundEffect.channel, soundEffect.sound, 0);
      soundEffect.active = false;
    }
  }
  for (auto& i : player1->hurtSoundEffects) {
    SoundObj& soundEffect = i.second;
    if (soundEffect.active) {
      Mix_PlayChannel(soundEffect.channel, soundEffect.sound, 0);
      soundEffect.active = false;
    }
    if (player1->currentHurtSoundID++ == player1->hurtSoundMax) {
      player1->currentHurtSoundID = 1;
    }
  }
  for (auto& entity : player1->entityList) {
    for (auto& i : entity.soundsEffects) {
      SoundObj& soundEffect = i.second;
      if (soundEffect.active) {
        Mix_PlayChannel(soundEffect.channel, soundEffect.sound, 0);
        soundEffect.active = false;
      }
    }
  }
  for (auto& i : player2->soundsEffects) {
    SoundObj& soundEffect = i.second;
    if (soundEffect.active) {
      Mix_PlayChannel(soundEffect.channel, soundEffect.sound, 0);
      soundEffect.active = false;
    }
  }
  for (auto& i : player2->hurtSoundEffects) {
    SoundObj& soundEffect = i.second;
    if (soundEffect.active) {
      Mix_PlayChannel(soundEffect.channel, soundEffect.sound, 0);
      soundEffect.active = false;
    }
    if (player2->currentHurtSoundID++ == player2->hurtSoundMax) {
      player2->currentHurtSoundID = 1;
    }
  }
  for (auto& entity : player2->entityList) {
    for (auto& i : entity.soundsEffects) {
      SoundObj& soundEffect = i.second;
      if (soundEffect.active) {
        Mix_PlayChannel(soundEffect.channel, soundEffect.sound, 0);
        soundEffect.active = false;
      }
    }
  }
}

void FightState::checkCorner(Character* player){
  if(player->getPos().first - player->width <= 0 || player->getPos().first + player->width >= 3840){
    player->inCorner = true;
  } else {
    player->inCorner = false;
  }
}

void FightState::checkHitstop(Character* player){
  if(player->inHitStop && --player->hitStop == 0){
    player->inHitStop = false;
  }
}

void FightState::checkEntityHitstop(Character* player){
  for (auto &i : player->entityList) {
    if(i.inHitStop && --i.hitStop == 0){
      i.inHitStop = false;
    }
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
            // find how deeply intersected they are
            bool p1Lefter = p1Pos.first < p2Pos.first;
            if (p1Pos.first == p2Pos.first) {
              p1Lefter = player1->faceRight;
            }

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

void FightState::checkThrowCollisions(){
  ThrowResult p1ThrowState  = checkThrowAgainst(player2, player1);
  ThrowResult p2ThrowState  = checkThrowAgainst(player1, player2);

  if(p1ThrowState.thrown && p2ThrowState.thrown) {
    int p1ThrowType = p1ThrowState.throwCb->throwType;
    int p2ThrowType = p2ThrowState.throwCb->throwType;
    
    player1->velocityX = 0;
    player1->velocityY = 0;
    player2->velocityX = 0;
    player2->velocityY = 0;

    if(p1ThrowType == 2 && p2ThrowType == 2){
      // grounded throw tech
      player1->changeState(55);
      player2->changeState(55);
    } else if(p1ThrowType == 1 && p2ThrowType == 1){
      // air throw tech
      player1->changeState(62);
      player2->changeState(62);
    }
  } else if (p1ThrowState.thrown) {
    player1->velocityX = 0;
    player1->velocityY = 0;
    player2->velocityX = 0;
    player2->velocityY = 0;

    if (player1->control) {
      player1->control = 0;

      player1->changeState(p1ThrowState.throwCb->techAttempt);
      player2->changeState(p1ThrowState.throwCb->throwAttempt);
    } else {
      player1->changeState(p1ThrowState.throwCb->opponentThrowSuccess);
      player2->changeState(p1ThrowState.throwCb->throwSuccess);
    }
  } else if (p2ThrowState.thrown){
    player1->velocityX = 0;
    player1->velocityY = 0;
    player2->velocityX = 0;
    player2->velocityY = 0;

    if (player2->control) {
      player2->control = 0;

      int throwAttempt = p2ThrowState.throwCb->throwAttempt;
      int techAttempt = p2ThrowState.throwCb->techAttempt;
      printf("2 had control: %d ", player2->control);
      printf("the throwCB throwAttempt: %d, the techAttempt:%d\n",throwAttempt, techAttempt);

      player2->changeState(techAttempt);
      player1->changeState(throwAttempt);
    } else {
      printf("player 2 didnt have control\n");
      player2->changeState(p2ThrowState.throwCb->opponentThrowSuccess);
      player1->changeState(p2ThrowState.throwCb->throwSuccess);
    }
  }
}

ThrowResult FightState::checkThrowAgainst(Character* thrower, Character* throwee){
  ThrowResult result = ThrowResult{false, NULL};
  bool canThrow = ( !thrower->currentState->hitboxesDisabled 
      && throwee->hitstun == 0 && throwee->blockstun == 0 
      && throwee->currentState->stateNum != 24 
      && throwee->currentState->stateNum != 35 
      && throwee->currentState->stateNum != 25 );

  if (canThrow) {
    for (auto p1ThrowHitbox : thrower->currentState->throwHitBoxes) {
      if(!p1ThrowHitbox->disabled){
        printf("checking a throwbox\n");
        for (auto p2HurtBox : throwee->currentState->throwHurtBoxes) {
          if(!p2HurtBox->disabled){
            if (CollisionBox::checkAABB(*p1ThrowHitbox, *p2HurtBox)) {
              if (p1ThrowHitbox->throwType == 1 && throwee->_getYPos() > 0) {
                result.thrown = true;
                result.throwCb = p1ThrowHitbox;
                // int success = p1ThrowHitbox->success;
                // int opponentState = p1ThrowHitbox->opponentState;

                thrower->frameLastAttackConnected = gameTime; 
                thrower->currentState->hitboxesDisabled = true;
                // thrower->changeState(success); 

                // throwee->comboCounter++;
                // throwee->hitstun = p1ThrowHitbox->hitstun;
                // throwee->changeState(opponentState);
                
              } else if(p1ThrowHitbox->throwType == 2 && throwee->_getYPos() == 0) {
                // int success = p1ThrowHitbox->success;
                // int opponentState = p1ThrowHitbox->opponentState;
                result.thrown = true;
                result.throwCb = p1ThrowHitbox;
                thrower->frameLastAttackConnected = gameTime; 
                thrower->currentState->hitboxesDisabled = true;
                // thrower->changeState(success); 

                // throwee->comboCounter++;
                // throwee->hitstun = p1ThrowHitbox->hitstun;
                // throwee->changeState(opponentState);
              }
            }
          }
        }
      }
    }
  }
  return result;
}

HitResult FightState::checkHitboxAgainstHurtbox(Character* hitter, Character* hurter){
  if (!hitter->currentState->hitboxesDisabled) {
    for (auto hitBox : hitter->currentState->hitBoxes) {
      bool groupDisabled = hitter->currentState->hitboxGroupDisabled[hitBox->groupID];
      if(!hitBox->disabled && !groupDisabled){
        for (auto hurtBox : hurter->currentState->hurtBoxes) {
          if(!hurtBox->disabled && !groupDisabled){
            if (CollisionBox::checkAABB(*hitBox, *hurtBox)) {
              CollisionRect hitsparkIntersect = CollisionBox::getAABBIntersect(*hitBox, *hurtBox);
              // printf("hitbox collision detected\n");
              hitter->inHitStop = true;
              hitter->hitStop = hitBox->hitstop;

              hurter->hitStop = hitBox->hitstop;
              hurter->inHitStop = true;
              hitter->frameLastAttackConnected = gameTime; 
              // TODO: Hitbox group IDs
              hitter->currentState->hitboxGroupDisabled[hitBox->groupID] = true;
              hitter->currentState->canHitCancel = true;
              hitter->_addMeter(hitBox->hitMeterGain);

              int hurterCurrentState = hurter->currentState->stateNum;
              bool blocking = (hurterCurrentState == 28 || hurterCurrentState == 29 || hurterCurrentState == 50);
              // printf("control? %d, blocking? %d, stateNum:%d\n", hurter->control, blocking, hurterCurrentState);
              int blocktype = hitBox->blockType;
              if((blocking && blocktype == 1) || (blocking && checkBlock(blocktype, hurter)) || (hurter->control && checkBlock(blocktype, hurter))){
                bool instantBlocked = hurter->_checkCommand(11);
                if (instantBlocked) {
                  Mix_PlayChannel(0, instantBlock, 0);
                  int realBlockstun = hitBox->blockstun - 4;
                  realBlockstun = realBlockstun <= 0 ? 1 : realBlockstun;
                  hurter->blockstun = realBlockstun;
                } else {
                  hurter->blockstun = hitBox->blockstun;
                }
                hurter->control = 0;
                if (hurter->_getYPos() > 0) {
                  hurter->changeState(50);
                } else {
                  switch (hitBox->blockType) {
                    case 1:
                      if (hurter->_getInput(1)) {
                        hurter->changeState(29);
                      } else {
                        hurter->changeState(28);
                      }
                      break;
                    case 2:
                      hurter->changeState(29);
                      break;
                    case 3:
                      hurter->changeState(28);
                      break;
                    // should throw error here
                    default: break;
                  }
                }
                // printf("ohh u got the blocksies?\n");
                if (hurter->inCorner) {
                  hitter->pushTime = hitBox->pushTime;
                  if (hitter->faceRight) {
                    hitter->pushBackVelocity = hitBox->pushback;
                  } else {
                    hitter->pushBackVelocity = -(hitBox->pushback/2);
                  }
                } else {
                  hurter->pushTime = hitBox->pushTime;
                  if (hitter->faceRight) {
                    hurter->pushBackVelocity = -hitBox->pushback;
                  } else {
                    hurter->pushBackVelocity = hitBox->pushback;
                  }
                }
                int xEdge = hurter->faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                int visualID = hitBox->guardsparkID;
                // printf("the visID %d\n", visualID);
                VisualEffect& visFX = hurter->guardSparks.at(visualID);
                visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                visFX.setActive(true);
                // printf("found guardSpark for hurter, the playLEngth %d\n", visFX.getPlayLength());
                hurter->soundsEffects.at(hitBox->guardSoundID).active = true;
                hurter->soundsEffects.at(hitBox->guardSoundID).channel = hurter->soundChannel + 2;
              } else {
                if (hurter->inCorner) {
                  hitter->pushTime = hitBox->hitPushTime;
                  if (hitter->faceRight) {
                    hitter->pushBackVelocity = hitBox->hitVelocityX/2;
                  } else {
                    hitter->pushBackVelocity = -(hitBox->hitVelocityX/2);
                  }
                } else {
                  hurter->hitPushTime = hitBox->hitPushTime;
                  if (hitter->faceRight) {
                    hurter->hitPushVelX = -hitBox->hitVelocityX;
                  } else {
                    hurter->hitPushVelX = hitBox->hitVelocityX;
                  }
                }

                bool wasACounter = hurter->currentState->counterHitFlag;
                // printf("is the hurter being couner hit?? time:  %d  counterhit: %d\n", hurter->currentState->stateTime, hurter->currentState->counterHitFlag);
                hurter->currentState->counterHitFlag = false;
                if (wasACounter) {
                  Mix_PlayChannel(0, countah, 0);
                }
                int xEdge = hitter->faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                int visualID = hitBox->hitsparkID;
                // printf("the visID %d\n", visualID);
                VisualEffect& visFX = hitter->hitSparks.at(visualID);
                visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                visFX.setActive(true);
                // printf("found hitspark for hitter, the playLEngth %d\n", visFX.getPlayLength());

                hurter->control = 0;
                int finalHitstun = wasACounter ? (hitBox->hitstun + 4) : (hitBox->hitstun); 
                finalHitstun += (hurter->_getYPos() > 0) ? 4 : 0;
                hurter->health -= wasACounter ? (hitBox->damage + (hitBox->damage * .2)) : (hitBox->damage);
                hurter->hitstun = finalHitstun;

                hurter->comboCounter++;
                hurter->hurtSoundEffects.at(hurter->currentHurtSoundID).active = true;
                hitter->soundsEffects.at(hitBox->hitSoundID).active = true;
                hitter->soundsEffects.at(hitBox->hitSoundID).channel = hitter->soundChannel + 2;

                int hurterCurrentState = hurter->currentState->stateNum;
                if(hitBox->hitType == LAUNCHER || hurter->_getYPos() > 0 
                    || hurterCurrentState  == 24 || hurterCurrentState == 35 
                    || hurterCurrentState == 52 || hurterCurrentState == 53){
                  if (hitBox->airHitstun > 0) {
                    hurter->hitstun = hitBox->airHitstun;
                  }
                  if (hitBox->airHitVelocityX > 0) {
                    if (hurter->inCorner) {
                      hitter->pushTime = hitBox->hitPushTime;
                      if (hitter->faceRight) {
                        hitter->pushBackVelocity = hitBox->airHitVelocityX/2;
                      } else {
                        hitter->pushBackVelocity = -(hitBox->airHitVelocityX/2);
                      }
                    } else {
                      hurter->hitPushTime = hitBox->hitPushTime;
                      if (hitter->faceRight) {
                        hurter->hitPushVelX = -hitBox->airHitVelocityX;
                      } else {
                        hurter->hitPushVelX = hitBox->airHitVelocityX;
                      }
                    }
                  }
                  hurter->velocityY = hitBox->hitVelocityY;
                  return {true, wasACounter, 24, NULL};
                } else {
                  return {true, wasACounter, 9, NULL};
                }
              }
            }
          }
        }
      }
    }
  }
  return {false, false, 0, NULL};
}

int FightState::checkProximityAgainst(Character* hitter, Character* hurter){
  if (hurter->getPos().second <= 0) {
    if (!hitter->currentState->hitboxesDisabled) {
      for (auto hitBox : hitter->currentState->proximityBoxes) {
        bool groupDisabled = hitter->currentState->hitboxGroupDisabled[hitBox->groupID];
        if(!hitBox->disabled && !groupDisabled){
          for (auto hurtBox : hurter->currentState->hurtBoxes) {
            if(!hurtBox->disabled && !groupDisabled){
              if (CollisionBox::checkAABB(*hitBox, *hurtBox)) {
                // printf("proximity collision detected\n");
                if (hurter->currentState->stateNum == 3) {
                  hurter->changeState(28);
                }
                if (hurter->currentState->stateNum == 4 && hurter->_getInput(1)) {
                  hurter->changeState(29);
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

int FightState::checkProjectileCollisions(Character* player1, Character* player2){
  for (auto &entity : player1->entityList) {
    if (entity.active && !entity.currentState->hitboxesDisabled) {
      for (auto entityHitbox : entity.currentState->projectileBoxes) {
        bool groupDisabled = entity.currentState->hitboxGroupDisabled[entityHitbox->groupID];
        if(!groupDisabled){
          for (auto &otherEntity : player2->entityList) {
            if (otherEntity.active && !otherEntity.currentState->hitboxesDisabled) {
              for (auto otherEntityHitbox : otherEntity.currentState->projectileBoxes) {
                bool groupDisabled = otherEntity.currentState->hitboxGroupDisabled[entityHitbox->groupID];
                if(!groupDisabled){
                  if (CollisionBox::checkAABB(*entityHitbox, *otherEntityHitbox)){
                    CollisionRect hitsparkIntersect = CollisionBox::getAABBIntersect(*entityHitbox, *otherEntityHitbox);
                    entity.inHitStop = true;
                    entity.hitStop = 6;
                    otherEntity.inHitStop = true;
                    otherEntity.hitStop = 6;
                    entity.currentState->hitboxGroupDisabled[entityHitbox->groupID] = true;
                    entity.currentState->canHitCancel = true;
                    otherEntity.currentState->hitboxGroupDisabled[entityHitbox->groupID] = true;
                    otherEntity.currentState->canHitCancel = true;
                    printf("entityProjectilesColliding\n");
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

void FightState::checkHitCollisions(){
  HitResult p2HitState = checkHitboxAgainstHurtbox(player1, player2);
  HitResult p1HitState = checkHitboxAgainstHurtbox(player2, player1);

  if (p1HitState.hit) {
    player1->changeState(p1HitState.hitState);
    if (p1HitState.counter) {
      printf("p1 counterHit!!\n");
      p1CounterHit.setStateTime(0);
      p1CounterHit.setActive(true);
    }
  }
  if (p2HitState.hit) {
    player2->changeState(p2HitState.hitState);
    if (p2HitState.counter) {
      printf("p2 counterHit!!\n");
      p2CounterHit.setStateTime(0);
      p2CounterHit.setActive(true);
    }
  }

  checkEntityHitCollisions();
}

HitResult FightState::checkEntityHitAgainst(Character* p1, Character* p2){
  bool p2Hit = false;
  for (auto &entity : p1->entityList) {
    if (entity.active && !entity.currentState->hitboxesDisabled) {
      for (auto entityHitbox : entity.currentState->projectileBoxes) {
        bool groupDisabled = entity.currentState->hitboxGroupDisabled[entityHitbox->groupID];
        if(!entityHitbox->disabled && !groupDisabled){
          for (auto p2HurtBox : p2->currentState->hurtBoxes) {
            if(!p2HurtBox->disabled){
              if (CollisionBox::checkAABB(*entityHitbox, *p2HurtBox)) {
                CollisionRect hitsparkIntersect = CollisionBox::getAABBIntersect(*entityHitbox, *p2HurtBox);
                // printf("entity hitbox collision detected\n");
                bool entityFaceRight = entity.faceRight;
                entity.inHitStop = true;
                entity.hitStop = entityHitbox->hitstop;

                p2->inHitStop = true;
                p2->hitStop = entityHitbox->hitstop;

                p1->frameLastAttackConnected = gameTime; 
                entity.currentState->hitboxGroupDisabled[entityHitbox->groupID] = true;
                // TODO: Hitbox group IDs
                entity.currentState->canHitCancel = true;

                int p2StateNum = p2->currentState->stateNum;
                if((p2StateNum == 28 || p2StateNum == 29 || p2StateNum == 50) || (p2->control && checkBlock(entityHitbox->blockType, p2))){
                  p2->control = 0;
                  bool instantBlocked = p2->_checkCommand(11);
                  if (instantBlocked) {
                    Mix_PlayChannel(0, instantBlock, 0);
                    int realBlockstun = entityHitbox->blockstun - 4;
                    realBlockstun = realBlockstun <= 0 ? 1 : realBlockstun;
                    p2->blockstun = realBlockstun;
                  } else {
                    p2->blockstun = entityHitbox->blockstun;
                  }
                  if (p2->_getYPos() > 0) {
                    // TODO: air blocking state
                    p2->changeState(50);
                  } else {
                    switch (entityHitbox->blockType) {
                      case 1:
                        if (p2->_getInput(1)) {
                          p2->changeState(29);
                        } else {
                          p2->changeState(28);
                        }
                        break;
                      case 2:
                        p2->changeState(29);
                        break;
                      case 3:
                        p2->changeState(28);
                        break;
                      // should throw error here
                      default: break;
                    }
                  }

                  p2->pushTime = entityHitbox->pushTime;
                  if(p2->faceRight == entityFaceRight){
                    if (p2->faceRight) {
                      p2->pushBackVelocity = -entityHitbox->pushback;
                    } else {
                      p2->pushBackVelocity = entityHitbox->pushback;
                    }
                  } else {
                    if (p2->faceRight) {
                      p2->pushBackVelocity = entityHitbox->pushback;
                    } else {
                      p2->pushBackVelocity = -entityHitbox->pushback;
                    }
                  }
                   
                  printf("ohh u got the blocksies?\n");
                  int xEdge = p2->faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                  int visualID = entityHitbox->guardsparkID;
                  VisualEffect& visFX = p2->guardSparks.at(visualID);
                  visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                  visFX.setActive(true);
                  // printf("found guardSpark for hurter, the playLEngth %d\n", visFX.getPlayLength());

                  p2->soundsEffects.at(entityHitbox->guardSoundID).active = true;
                  p2->soundsEffects.at(entityHitbox->guardSoundID).channel = p2->soundChannel + 2;
                } else {
                  p2->hitPushTime = entityHitbox->hitPushTime;
                  printf("whats the entitie's hit vel? %d\n", entityHitbox->hitVelocityX);
                  if(p2->faceRight == entityFaceRight){
                    if (p2->faceRight) {
                      p2->hitPushVelX = -entityHitbox->hitVelocityX;
                    } else {
                      p2->hitPushVelX = entityHitbox->hitVelocityX;
                    }
                  } else {
                    if (p2->faceRight) {
                      p2->hitPushVelX = entityHitbox->hitVelocityX;
                    } else {
                      p2->hitPushVelX = -entityHitbox->hitVelocityX;
                    }
                  }
                  bool wasACounter = p2->currentState->counterHitFlag;
                  printf("is the hurter being couner hit?? time:  %d  counterhit: %d\n", p2->currentState->stateTime, p2->currentState->counterHitFlag);
                  p2->currentState->counterHitFlag = false;
                  if (wasACounter) {
                    Mix_PlayChannel(0, countah, 0);
                  }

                  int xEdge = entity.faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                  int visualID = entityHitbox->hitsparkID;
                  VisualEffect& visFX = entity.hitSparks.at(visualID);
                  visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                  visFX.setActive(true);
                  printf("found hitspark for hitter, the playLEngth %d\n", visFX.getPlayLength());


                  printf("ya wasnt blockin kid\n");
                  p2->control = 0;
                  p2->health -= entityHitbox->damage;
                  p2->hitstun = entityHitbox->hitstun;
                  p2->comboCounter++;
                  p2->hurtSoundEffects.at(p2->currentHurtSoundID).active = true;
                  entity.soundsEffects.at(entityHitbox->hitSoundID).active = true;
                  entity.soundsEffects.at(entityHitbox->hitSoundID).channel = p1->soundChannel + 2;

                  if(entityHitbox->hitType == LAUNCHER || p2->_getYPos() > 0 || p2->currentState->stateNum == 24){
                    p2->velocityY = entityHitbox->hitVelocityY;
                    return {true, false, 24, NULL};
                  } else {
                    return {true, false, 9, NULL};
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return {false, false, 0, NULL};
}

void FightState::checkEntityHitCollisions(){
  // TODO: func
  HitResult p2Hit = checkEntityHitAgainst(player1, player2);
  HitResult p1Hit = checkEntityHitAgainst(player2, player1);

  if (p1Hit.hit) {
    player1->changeState(p1Hit.hitState);
  }
  if (p2Hit.hit) {
    player2->changeState(p2Hit.hitState);
  }

}

bool FightState::checkBlock(int blockType, Character* player){
  bool isHoldingDownBack = player->_getInput(1);
  bool isHoldingBack = player->_getInput(4);
  bool upBackinScrub = player->_getInput(7);
  printf("player2 is holding downback? %d, what about back %d\n", isHoldingDownBack, isHoldingBack);
  // I know, enum
  if (player->_getYPos() > 0) {
    if(isHoldingBack || isHoldingDownBack || upBackinScrub){
      return true;
    }
  }
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
  if(player1->getPos().first - player1->width < 0) {
    player1->setXPos(0+player1->width);
    player1->updateCollisionBoxPositions();
  }
  if (player1->getPos().first - player1->width < camera.lowerBound) {
    // printf("why am I outside the camera on the left?\n");
    player1->setXPos(camera.lowerBound+player1->width);
    player1->updateCollisionBoxPositions();
  }

  if(player1->getPos().first + player1->width > 3840) {
    player1->setXPos(3840 - player1->width);
    player1->updateCollisionBoxPositions();
  }
  if (player1->getPos().first + player1->width > camera.upperBound) {
    // printf("why am I outside the camera on the right?\n");
    player1->setXPos(camera.upperBound - player1->width);
    player1->updateCollisionBoxPositions();
  }

  if(player2->getPos().first - player2->width < 0) {
    player2->setXPos(0 + player2->width);
    player2->updateCollisionBoxPositions();
  }
  if (player2->getPos().first - player2->width < camera.lowerBound) {
    player2->setXPos(camera.lowerBound + player2->width);
    player2->updateCollisionBoxPositions();
  }

  if(player2->getPos().first + player2->width > 3840) {
    player2->setXPos(3840 - player2->width);
    player2->updateCollisionBoxPositions();
  }
  if (player2->getPos().first + player2->width > camera.upperBound) {
    player2->setXPos(camera.upperBound - player2->width);
    player2->updateCollisionBoxPositions();
  }
}

void FightState::checkHealth(){
  // TODO: if training mode
  // TODO: refactor jesus why are you like this
  if (player2->currentState->stateNum == 1 && player2->currentState->stateTime == 20) {
    player2->health = 100;
    player2->redHealth = 100;
    player2->redHealthCounter = 0;
  }
  if (player1->currentState->stateNum == 1 && player1->currentState->stateTime == 20) {
    player1->health = 100;
    player1->redHealth = 100;
    player1->redHealthCounter = 0;
  }

  if (player1->tension > player1->maxTension) {
    player1->tension = player1->maxTension;
  }
  if (player2->tension > player2->maxTension) {
    player2->tension = player2->maxTension;
  }

  if (player1->meter >= player1->maxMeter) {
    player1->meter = player1->maxMeter;
  }
  if (player2->meter >= player2->maxMeter) {
    player2->meter = player2->maxMeter;
  }
  if (player1->meter < 0 ) {
    player1->meter = 0;
  }
  if (player2->meter < 0 ) {
    player2->meter = 0;
  }
  if ((player1->health <= 0 || player2->health <= 0) && (!player1->isDead && !player2->isDead)) {
    knockoutPopup.setX(camera.middle);
    knockoutPopup.setY(camera.cameraRect.y);
    knockoutPopup.setStateTime(0);
    knockoutPopup.setActive(true);
    Mix_PlayChannel(0, koSound, 0);
    if (player1->health <= 0 && player1->hitstun >= 1) {
      player1->isDead = true;
      p2RoundsWon++;
      printf("p2RoundsWon:%d\n", p2RoundsWon);
      roundWinner = 2;
    }
    if (player2->health <= 0 && player2->hitstun >= 1) {
      player2->isDead = true;
      p1RoundsWon++;
      roundWinner = 1;
      printf("p1RoundsWon:%d\n", p1RoundsWon);
    }
    currentRound++;
    slowMode = true;
  }

  if (player1->comboCounter == 0 && player1->health < player1->redHealth) {
    if (player1->redHealthCounter++ == 3) {
      player1->redHealthCounter = 0;
      player1->redHealth--;
    }
  }
  if (player2->comboCounter == 0 && player2->health < player2->redHealth) {
    if (player2->redHealthCounter++ == 3) {
      player2->redHealthCounter = 0;
      player2->redHealth--;
    }
  }

  if (roundEnd) {
    if (slowDownCounter++ == 180) {
      slowDownCounter = 0;
      roundEnd = false;
      if (p2RoundsWon == 2 && p1RoundsWon == 2) {
        p1RoundsWon = 1;
        p2RoundsWon = 1;
      } else if (p1RoundsWon == 2){
        printf("p1 won\n");
        stateManager->popState();
      } else if (p2RoundsWon == 2){
        printf("p2 won\n");
        stateManager->popState();
      } else {
        printf("restarting round\n");
        restartRound();
      }
    }
  }
}

void FightState::restartRound(){
  player1->refresh();
  player2->refresh();
  player1->control = 0;
  player2->control = 0;

  player1->setXPos(1700);
  player1->setYPos(0);

  player2->setXPos(2200);
  player2->setYPos(0);
  camera.update(1700, 2200);
  roundStartCounter = 210;
  roundStart = true;
}

void FightState::updateFaceRight(){
  if (player1->getPos().first == player2->getPos().first) {
  } else {
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
  float p1HpPercent = (float)p1Hp / (float)player1->maxHealth;
  float p1RedPercent = (float)player1->redHealth/(float)player1->maxHealth;

  int p2Hp = player2->health;
  float p2HpPercent = (float)p2Hp / (float)player2->maxHealth;
  float p2RedPercent = (float)player2->redHealth/(float)player2->maxHealth;

  // printf("the red health percentage: %f\n", p2RedPercent);
  currentScreen.renderHealthBar(p1HpPercent, p1RedPercent, true);
  currentScreen.renderHealthBar(p2HpPercent, p2RedPercent, false);
  // render meters
  int p1Meter = player1->meter;
  float p1MeterPercent = (float)p1Meter / (float)player1->maxMeter;
  int p2Meter = player2->meter;
  float p2MeterPercent = (float)p2Meter / (float)player2->maxMeter;
  currentScreen.renderMeterBar(p1MeterPercent, true);
  currentScreen.renderMeterBar(p2MeterPercent, false);
}

void FightState::renderComboCount(){
  int p1ComboCount = player1->comboCounter;
  int p2ComboCount = player2->comboCounter;
  if(p2ComboCount > 1){
    currentScreen.renderComboCount(true, p2ComboCount);
  }
  if(p1ComboCount > 1){
    currentScreen.renderComboCount(false, p1ComboCount);
  }
}

void FightState::renderInputHistory(){
  // printf("p1InputEventList size %ld\n", player1->virtualController->inputEventList.size());
  currentScreen.renderInputHistory(true, player1->virtualController->inputEventList);
  currentScreen.renderInputHistory(false, player2->virtualController->inputEventList);
}
