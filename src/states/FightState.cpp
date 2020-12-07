#include "states/FightState.h"
#include "screens/FightScreen.h"
#include "game_objects/Character.h"
#include "game_objects/Stage.h"
#include "game_objects/Entity.h"
#include <chrono>
#include <thread>
#include <csignal>
#include <boost/format.hpp>
// #define SYNC_TEST    // test: turn on synctest



FightState* ggpoFightState;
Character* characters[2];
int checksum = 0;
//TODO:: COORDINATE_SCALE
int worldWidth = 3840*100;
int p1StartPos = 1700*100;
int p2StartPos = 2200*100;
int camWidth = 1280*100;
int camHeight = 720*100;


bool blockState(int playerNum, int state){
return ( state == characters[playerNum]->specialStateMap[SS_BLOCK_STAND]
          || state == characters[playerNum]->specialStateMap[SS_BLOCK_CROUCH]
          || state == characters[playerNum]->specialStateMap[SS_AIR_BLOCK]
          || state == characters[playerNum]->specialStateMap[SS_PUSH_BLOCK]
          || state == characters[playerNum]->specialStateMap[SS_CROUCH_PUSH_BLOCK]
          || state == characters[playerNum]->specialStateMap[SS_AIR_PUSH_BLOCK]
      );
}  

bool airHurtState(int playerNum, int state){
  return (
      state == characters[playerNum]->specialStateMap[SS_AIR_HURT]
      || state == characters[playerNum]->specialStateMap[SS_AIR_HURT_RECOVERY]
      || state == characters[playerNum]->specialStateMap[SS_BLOWBACK_FALLING]
      || state == characters[playerNum]->specialStateMap[SS_DEAD_STANDING]
      || state == characters[playerNum]->specialStateMap[SS_DEAD_KNOCKDOWN]
      || state == characters[playerNum]->specialStateMap[SS_GROUNDBOUNCE_FLING]
      || state == characters[playerNum]->specialStateMap[SS_GROUNDBOUNCE_IMPACT]);
}

unsigned createMask(unsigned a, unsigned b){
   unsigned r = 0;
   for (unsigned i=a; i<=b; i++)
       r |= 1 << i;

   return r;
}

int intFromInput(InputEvent event){
  int returnInt = 0;
  returnInt = event.inputBit;
  if (event.pressed) {
   returnInt = ((1 << 30) | returnInt); 
  }
  return returnInt;
}

InputEvent intToInputEvent(int input){
  unsigned mask = createMask(0, 15);
  uint16_t inputBit = mask & input;
  return InputEvent(input, (1 == ( (input >> 30) & 1)));
}

/*
 * Simple checksum function stolen from wikipedia:
 *
 *   http://en.wikipedia.org/wiki/Fletcher%27s_checksum
 */

int fletcher32_checksum(short *data, size_t len) {
  int sum1 = 0xffff, sum2 = 0xffff;

  while (len) {
    unsigned tlen = len > 360 ? 360 : len;
    len -= tlen;
    do {
       sum1 += *data++;
       sum2 += sum1;
    } while (--tlen);
    sum1 = (sum1 & 0xffff) + (sum1 >> 16);
    sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  }

  /* Second reduction step to reduce sums to 16 bits */
  sum1 = (sum1 & 0xffff) + (sum1 >> 16);
  sum2 = (sum2 & 0xffff) + (sum2 >> 16);
  return sum2 << 16 | sum1;
}


bool fsBeginGame(const char* game){
  return ggpoFightState->beginGame(game);
}

bool fsAdvanceFrame(int flags){
  printf("ggpo advance frame called\n");
  int inputs[2] = {0};
  int disconnectFlags;
  VirtualController* p1Vc = ggpoFightState->player1->virtualController;
  VirtualController* p2Vc = ggpoFightState->player2->virtualController;

  GGPOSession* ggpoObj = ggpoFightState->ggpo;
  ggpo_synchronize_input(ggpoObj, (void *)inputs, sizeof(int)*2, &disconnectFlags);
  p1Vc->inputHistory.front().clear();
  p2Vc->inputHistory.front().clear();

  // simulate a local keypress with input
  p1Vc->setState(inputs[0]);
  p1Vc->addNetInput();

  p2Vc->setState(inputs[1]);
  p2Vc->addNetInput();
  
  ggpoFightState->shouldUpdate = true;
  ggpoFightState->inAdvanceState = true;

  ggpoFightState->handleInput();
  ggpoFightState->update();
  return true;
}

bool fsLoadGameState(unsigned char* buffer, int length){
  ggpoFightState->loadState(buffer, length);
  return true;
}

bool fsSaveGameState(unsigned char** buffer, int* len, int* checksum, int frame){
  printf("saving game state\n");
  ggpoFightState->saveState(buffer, len, frame);
  return true;
}

void fsFreeBuffer(void* buffer){ 
  free(buffer);
}


bool fsOnEvent(GGPOEvent* info){ 
  switch (info->code) {
    case GGPO_EVENTCODE_CONNECTED_TO_PEER:
      printf("GGPO_EVENT done connecting to peer\n");
      break;
    case GGPO_EVENTCODE_SYNCHRONIZING_WITH_PEER:
      printf("GGPO_EVENT synchronizing with peer...\n");
      break;
    case GGPO_EVENTCODE_SYNCHRONIZED_WITH_PEER:
      printf("GGPO_EVENT done syncing with peer\n");
      break;
    case GGPO_EVENTCODE_RUNNING:
      printf("GGPO_EVENT running\n");
      ggpoFightState->doneSync = true;
      break;
    case GGPO_EVENTCODE_DISCONNECTED_FROM_PEER:
      printf("GGPO_EVENT disconnected from peer\n");
      break;
    case GGPO_EVENTCODE_TIMESYNC:
      printf("GGPO_EVENT timesync\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000 * info->u.timesync.frames_ahead / 60));
      break;
    case GGPO_EVENTCODE_CONNECTION_INTERRUPTED:
      printf("GGPO_EVENT connection interrupted\n");
      break;
    case GGPO_EVENTCODE_CONNECTION_RESUMED:
      printf("GGPO_EVENT connection resumed\n");
      break;
    default:
      printf("GGPO_EVENT\n");
      break;
  }
  return true;
}



FightState::FightState(){ 
  printf("creating new fightState\n");
  stateName = "FIGHT_STATE";
  // bgMusic = Mix_LoadMUS("../data/audio/fightingTheme.mp3");
  Mix_VolumeMusic(25);
  Mix_Volume(0, 40);

  for (int i = 1; i < 8; ++i) {
    Mix_Volume(i, 16);
  }

  yawl_ready = Mix_LoadWAV("../data/audio/uiSounds/yawl_ready.mp3");
  round1Sound = Mix_LoadWAV("../data/audio/uiSounds/round_1.mp3");
  round2Sound = Mix_LoadWAV("../data/audio/uiSounds/round_2.mp3");
  finalRoundSound = Mix_LoadWAV("../data/audio/uiSounds/final_round.mp3");
  fightSound = Mix_LoadWAV("../data/audio/uiSounds/fight.mp3");
  koSound = Mix_LoadWAV("../data/audio/uiSounds/KO.mp3");
  p1WinSound = Mix_LoadWAV("../data/audio/uiSounds/player_1_wins.mp3");
  p2WinSound = Mix_LoadWAV("../data/audio/uiSounds/player_2_win.mp3");
  countah = Mix_LoadWAV("../data/audio/uiSounds/countah_1.mp3");
  instantBlock = Mix_LoadWAV("../data/audio/uiSounds/instantBlock.mp3");
  throwtech = Mix_LoadWAV("../data/audio/uiSounds/throwtech.mp3");
  pushBlock = Mix_LoadWAV("../data/characters/audio/heavyslice_guard2.wav");

  if(bgMusic == NULL) {
    printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
  }
}

FightState::~FightState(){
  printf("in fightState destructor\n");
}

void FightState::enter(){
  printf("entering fightState\n");
  netPlayState = stateManager->getNetplay();
  pnum = stateManager->getPnum();
  player1 = new Character(std::make_pair(p1StartPos,0), 1);
  player1->virtualController = inputManager->getVirtualController(0);
  player1->virtualController->initCommandCompiler();
  player1->soundChannel = 2;
  player1->gravityVal = 1;
  charStateManager->registerCharacter(player1, 1);

  player2 = new Character(std::make_pair(p2StartPos,0), 2);
  player2->virtualController = inputManager->getVirtualController(1);
  player2->virtualController->initCommandCompiler();
  player2->soundChannel = 5;
  player2->gravityVal = 1;
  charStateManager->registerCharacter(player2, 2);
  characters[0] = player1;
  characters[1] = player2;


  player1->otherChar = player2;
  player2->otherChar = player1;
  player1->control = 0;
  player2->control = 0;

  std::string p1DefPath = "../data/characters/" + stateManager->getCharName(1) + "/def.json";
  std::string p2DefPath = "../data/characters/" + stateManager->getCharName(2) + "/def.json";
  printf("player1 defpath %s\n", p1DefPath.c_str());
  player1->charName = stateManager->getCharName(1);
  player2->charName = stateManager->getCharName(2);
  player1->init(p1DefPath.c_str());
  player1->loadCustomStates(p2DefPath.c_str());
  player2->init(p2DefPath.c_str());
  player2->loadCustomStates(p1DefPath.c_str());

  printf("done loading characters\n");

  graphics->setCamera(&camera);
  camera.init(camWidth, camHeight, worldWidth);
  updateCamera();
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
  printf("done loading popups\n");

  if (netPlayState) {
    printf("starting ggpo init\n");
    shouldUpdate = false;
    if (pnum != 1) {
      inputManager->stickToVC[inputManager->p1SDLController] = player2->virtualController;
    }
    ggpoInit();
    printf("end ggpo init\n");
  }
}

void FightState::exit(){ 
  printf("exiting fight state\n");
  ggpo_close_session(ggpo);
  stateManager->setNetplay(false);
  delete player1;
  delete player2;
  delete this;
}

void FightState::pause(){ }
void FightState::resume(){ }

void FightState::saveState(unsigned char** buffer, int* length, int frame){
  FightStateObj saveObj;

  saveObj.currentRound = currentRound;
  saveObj.inSlowDown = inSlowDown;
  saveObj.p1RoundsWon = p1RoundsWon;
  saveObj.p2RoundsWon = p2RoundsWon;
  saveObj.roundEnd = roundEnd;
  saveObj.roundStart = roundStart;
  saveObj.roundStartCounter = roundStartCounter;
  saveObj.roundWinner = roundWinner;
  saveObj.screenFreeze = screenFreeze;
  saveObj.screenFreezeCounter = screenFreezeCounter;
  saveObj.screenFreezeLength = screenFreezeLength;
  saveObj.slowDownCounter = slowDownCounter;
  saveObj.shouldUpdate = shouldUpdate;

  saveObj.char1State = player1->saveState();
  saveObj.char2State = player2->saveState();
  saveObj.cameraState = camera.saveState();

  *length = sizeof(saveObj);
  *buffer = (unsigned char*) malloc(*length);
  memcpy(*buffer, &saveObj, *length);
  checksum = fletcher32_checksum((short *)*buffer, *length / 2);
}

void FightState::loadState(unsigned char* buffer, int length){
  FightStateObj saveObj;
  memcpy(&saveObj, buffer, length);

  currentRound = saveObj.currentRound;
  slowDownCounter = saveObj.slowDownCounter;
  p1RoundsWon = saveObj.p1RoundsWon;
  p2RoundsWon = saveObj.p2RoundsWon;
  roundEnd = saveObj.roundEnd;

  roundStart = saveObj.roundStart;
  inSlowDown = saveObj.inSlowDown;
  roundStartCounter = saveObj.roundStartCounter;
  roundWinner = saveObj.roundWinner;
  screenFreeze = saveObj.screenFreeze;
  screenFreezeCounter = saveObj.screenFreezeCounter;
  screenFreezeLength = saveObj.screenFreezeLength;
  shouldUpdate = saveObj.shouldUpdate;

  player1->loadState(saveObj.char1State);
  player2->loadState(saveObj.char2State);
  camera.loadState(saveObj.cameraState);
}


void FightState::handleInput(){
  if (netPlayState && doneSync) {
    if (!inAdvanceState) {
      netPlayHandleInput();
    } else {
      inAdvanceState = false;
    }
  }
  if (shouldUpdate) {
    if(!slowMode && !screenFreeze){
      handleRoundStart();
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
      checkThrowTechs();
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
    checkCorner(player1);
    checkCorner(player2);
    checkBounds();
    updateFaceRight();
  }
}

void FightState::update(){
  if (shouldUpdate) {
    if(!slowMode && !screenFreeze){
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
      if (player1->currentState->checkFlag(SUPER_ATTACK) && (player1->currentState->stateTime == player1->currentState->freezeFrame)) {
        screenFreeze = true;
        screenFreezeLength = player1->currentState->freezeLength;
        player1->activateVisFX(1);
      }
      if (player2->currentState->checkFlag(SUPER_ATTACK) && (player2->currentState->stateTime == player2->currentState->freezeFrame)) {
        screenFreeze = true;
        screenFreezeLength = player2->currentState->freezeLength;
        player2->activateVisFX(1);
      }
    }


    checkBounds();
    updateFaceRight();
    checkCorner(player1);
    checkCorner(player2);

    checkPushCollisions();
    checkBounds();

    updateCamera();
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

    if (screenFreeze) {
      if (screenFreezeCounter++ == screenFreezeLength) {
        screenFreezeCounter = 0;
        screenFreezeLength = 0;
        screenFreeze = false;
      }
    }
    updateVisuals();
    
    if (netPlayState && doneSync) {
      printf("calling ggpo advance frame\n");
      ggpo_advance_frame(ggpo);
    } else if(!netPlayState){
      saveState(&buffer, &bufferLen, gameTime);
    }
  }
  frameCount++;
  // printf("camera: x%d|y%d|UpperBound%d|LowerBound%d\n", camera.positionObj.x, camera.positionObj.y, camera.upperBound, camera.lowerBound);
  // printf("player1: x%d|y%d\n", player1->position.first, player1->position.second);
  // printf("player2: x%d|y%d\n", player2->position.first, player2->position.second);
}

void FightState::drawText(){
  if (player1->virtualController->copyMode) {
    if (player1->virtualController->copyModeSlot == 1) {
      currentScreen.recordStatus = RECORDING_ONE;
    } else {
      currentScreen.recordStatus = RECORDING_TWO;
    }
  } else if (player2->virtualController->playbackMode) {
    if (player1->virtualController->copyModeSlot == 1) {
      currentScreen.recordStatus = PLAYBACK_ONE;
    } else {
      currentScreen.recordStatus = PLAYBACK_TWO;
    }
  } else {
    currentScreen.recordStatus = RECORDING_NONE;
  }
  if (screenFreeze) {
    currentScreen.showGradient = true;
  } else {
    currentScreen.showGradient = false;
  }
  if (netPlayState && !doneSync) {
    currentScreen.recordStatus = RecordingStatus::CONNECTING;
  } else {
    currentScreen.recordStatus = RecordingStatus::RECORDING_NONE;
  }
  currentScreen.checksumValue = checksum;
  currentScreen.fps = graphics->getFPS();
}

void FightState::drawCounterHit(){
  if (p1CounterHit.getActive()) {
    p1CounterHit.draw();
  }
  if (p2CounterHit.getActive()) {
    p2CounterHit.draw();
  }
}


void FightState::drawUIEffects(){
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
}

void FightState::drawPlayers(){
  if (player1->frameLastAttackConnected > player2->frameLastAttackConnected) {
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
  } else {
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
  }
}

void FightState::handleSoundEffects(){
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

void FightState::draw() {
  drawText();
  currentScreen.draw();
  currentScreen.drawWins(p1RoundsWon, p2RoundsWon);
  drawCounterHit();
  drawHealthBars();
  drawComboCount();
  drawInputHistory();
  drawUIEffects();
  drawPlayers();
  handleSoundEffects();
}

void FightState::updateCamera(){
  camera.update(player1->getPos(), player2->getPos());
}

void FightState::updateVisuals(){
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

void FightState::checkCorner(Character* player){
  if(player->getPos().first - player->width <= 0 || player->getPos().first + player->width >= worldWidth){
    player->inCorner = true;
  } else {
    player->inCorner = false;
  }
}

void FightState::handleRoundStart(){
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
}

void FightState::checkThrowTechs(){
  if (player1->currentState->checkFlag(TECHABLE)) {
    if (player1->_checkCommand(5)) {
      int techState = player1->currentState->techState;
      player1->changeState(techState);
      player2->changeState(techState);

      player1->inHitStop = true;
      player2->inHitStop = true;
      player1->hitStop = 20;
      player2->hitStop = 20;
      Mix_PlayChannel(0, throwtech, 0);
    }
  }
  if (player2->currentState->checkFlag(TECHABLE)) {
    if (player2->_checkCommand(5)) {
      int techState = player2->currentState->techState;
      player1->changeState(techState);
      player2->changeState(techState);

      player1->inHitStop = true;
      player2->inHitStop = true;
      player1->hitStop = 20;
      player2->hitStop = 20;
      Mix_PlayChannel(0, throwtech, 0);
    }
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
              if ((p2Pos.first+player2->width) + (depth/2) > worldWidth) {
                int remainder = worldWidth - (p2Pos.first + (depth/2));
                player2->setXPos(worldWidth-player2->width);
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
              if ((p1Pos.first+player1->width) + (depth/2) > worldWidth) {
                int remainder = worldWidth - (p1Pos.first + (depth/2));
                player1->setXPos(worldWidth+player1->width);
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

void FightState::handleSameFrameThrowTech(SpecialState techState){
  player1->control = 0;
  player2->control = 0;
  player1->changeState(characters[0]->specialStateMap[techState]);
  player2->changeState(characters[1]->specialStateMap[techState]);
  player1->inHitStop = true;
  player2->inHitStop = true;
  player1->hitStop = 20;
  player2->hitStop = 20;
  Mix_PlayChannel(0, throwtech, 0);
}

void FightState::checkThrowCollisions(){
  ThrowResult p1ThrownState  = checkThrowAgainst(player2, player1);
  ThrowResult p2ThrownState  = checkThrowAgainst(player1, player2);

  if(p1ThrownState.thrown && p2ThrownState.thrown) {
    int p1ThrowType = p1ThrownState.throwCb->throwType;
    int p2ThrowType = p2ThrownState.throwCb->throwType;
    if(p1ThrowType == 2 && p2ThrowType == 2){
      handleSameFrameThrowTech(SS_AIR_THROW_TECH);
    } else if(p1ThrowType == 1 && p2ThrowType == 1){
      handleSameFrameThrowTech(SS_THROW_TECH);
    }
  } else if (p1ThrownState.thrown) {
    player1->velocityX = 0;
    player1->velocityY = 0;
    player2->velocityX = 0;
    player2->velocityY = 0;

    if (player1->control) {
      player1->control = 0;
      // you were thrown
      // TODO: opponentThrowSuccess is a confusing name
      // FIXME: CUSTOMSTATE
      int customStateOffset = player1->stateCount + p1ThrownState.throwCb->opponentThrowSuccess;
      player1->changeState(customStateOffset);

      player2->changeState(p1ThrownState.throwCb->throwSuccess);
    } else {
      // you were thrown
      // TODO: opponentThrowSuccess is a confusing name
      // FIXME: CUSTOMSTATE
      int customStateOffset = player1->stateCount + p1ThrownState.throwCb->opponentThrowSuccess;
      player1->changeState(customStateOffset);

      player2->changeState(p1ThrownState.throwCb->throwSuccess);
    }
  } else if (p2ThrownState.thrown){
    player1->velocityX = 0;
    player1->velocityY = 0;
    player2->velocityX = 0;
    player2->velocityY = 0;

    if (player2->control) {
      player2->control = 0;

      int throwAttempt = p2ThrownState.throwCb->throwAttempt;
      int techAttempt = p2ThrownState.throwCb->techAttempt;

      // FIXME: CUSTOMSTATE
      player2->changeState(techAttempt);
      player1->changeState(throwAttempt);
    } else {
      // FIXME: CUSTOMSTATE
      player2->changeState(p2ThrownState.throwCb->opponentThrowSuccess);

      player1->changeState(p2ThrownState.throwCb->throwSuccess);
    }
  }
}

ThrowResult FightState::checkThrowAgainst(Character* thrower, Character* throwee){
  ThrowResult result = ThrowResult{false, NULL};
  bool canThrow = ( !thrower->currentState->hitboxesDisabled 
      && throwee->hitstun == 0 && throwee->blockstun == 0 
      && !throwee->hurtState(throwee->currentState->stateNum) );

  if (canThrow) {
    for (auto p1ThrowHitbox : thrower->currentState->throwHitBoxes) {
      if(!p1ThrowHitbox->disabled){
        for (auto p2HurtBox : throwee->currentState->throwHurtBoxes) {
          if(!p2HurtBox->disabled){
            if (CollisionBox::checkAABB(*p1ThrowHitbox, *p2HurtBox)) {
              if (p1ThrowHitbox->throwType == 1 && throwee->_getYPos() > 0) {
                result.thrown = true;
                result.throwCb = p1ThrowHitbox;
                thrower->frameLastAttackConnected = gameTime; 
                thrower->currentState->hitboxesDisabled = true;
                
              } else if(p1ThrowHitbox->throwType == 2 && throwee->_getYPos() == 0) {
                result.thrown = true;
                result.throwCb = p1ThrowHitbox;
                thrower->frameLastAttackConnected = gameTime; 
                thrower->currentState->hitboxesDisabled = true;
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
              bool blocking = blockState(hurter->playerNum - 1, hurterCurrentState);
              int blocktype = hitBox->blockType;
              if((blocking && blocktype == 1) 
                  || (blocking && checkBlock(blocktype, hurter)) 
                  || (hurter->control && checkBlock(blocktype, hurter))){
                bool instantBlocked = hurter->_checkCommand(11);
                if (instantBlocked) {
                  hurter->isLight = true;
                  Mix_PlayChannel(0, instantBlock, 0);
                  int realBlockstun = hitBox->blockstun - 4;
                  realBlockstun = realBlockstun <= 0 ? 1 : realBlockstun;
                  hurter->blockstun = realBlockstun;
                } else {
                  hurter->blockstun = hitBox->blockstun;
                }
                if (hurterCurrentState == hurter->specialStateMap[SS_PUSH_BLOCK] 
                    || hurterCurrentState == hurter->specialStateMap[SS_CROUCH_PUSH_BLOCK] 
                    ||  hurterCurrentState == hurter->specialStateMap[SS_AIR_PUSH_BLOCK]) {
                  hurter->isGreen = true;
                  hurter->blockstun = hitBox->blockstun + 4;
                }
                hurter->control = 0;
                bool holdingButtons = hurter->_getInput(11) && hurter->_getInput(12);
                bool downBack = hurter->_getInput(1);
                bool backOrUpback = (hurter->_getInput(4) || hurter->_getInput(7)) && !downBack;
                bool anyBack = (hurter->_getInput(1) || hurter->_getInput(4) || hurter->_getInput(7));

                bool crouchPB = holdingButtons && downBack;
                bool standPB = holdingButtons && backOrUpback;
                if (hurter->_getYPos() > 0) {
                  if(anyBack && holdingButtons) {
                    Mix_PlayChannel(0, pushBlock, 0);
                    hurter->changeState(hurter->specialStateMap[SS_AIR_PUSH_BLOCK]);
                    hurter->_subtractMeter(20);
                  } else {
                    hurter->changeState(hurter->specialStateMap[SS_AIR_BLOCK]);
                  }
                } else {
                  switch (hitBox->blockType) {
                    case 1:
                      if (hurter->_getInput(1)) {
                        if (crouchPB) {
                          Mix_PlayChannel(0, pushBlock, 0);
                          hurter->changeState(hurter->specialStateMap[SS_CROUCH_PUSH_BLOCK]);
                          hurter->_subtractMeter(20);
                        } else {
                          hurter->changeState(hurter->specialStateMap[SS_BLOCK_CROUCH]);
                        }
                      } else {
                        if (standPB) {
                          hurter->_subtractMeter(20);
                          Mix_PlayChannel(0, pushBlock, 0);
                          hurter->changeState(hurter->specialStateMap[SS_PUSH_BLOCK]);
                        } else {
                          hurter->changeState(hurter->specialStateMap[SS_BLOCK_STAND]);
                        }
                      }
                      break;
                    case 2:
                      if (crouchPB) {
                        Mix_PlayChannel(0, pushBlock, 0);
                        hurter->_subtractMeter(20);
                        hurter->changeState(hurter->specialStateMap[SS_CROUCH_PUSH_BLOCK]);
                      } else {
                        hurter->changeState(hurter->specialStateMap[SS_BLOCK_CROUCH]);
                      }
                      break;
                    case 3:
                      if (standPB) {
                        Mix_PlayChannel(0, pushBlock, 0);
                        hurter->_subtractMeter(20);
                        hurter->changeState(hurter->specialStateMap[SS_PUSH_BLOCK]);
                      }
                      hurter->changeState(hurter->specialStateMap[SS_BLOCK_STAND]);
                      break;
                    // should throw error here
                    default: break;
                  }
                }
                if (hurter->inCorner) {
                  hitter->pushTime = hitBox->pushTime;
                  if (hitter->faceRight) {
                    hitter->pushBackVelocity = hitBox->pushback;
                  } else {
                    hitter->pushBackVelocity = -(hitBox->pushback);
                  }
                } else {
                  hurter->pushTime = hitBox->pushTime;
                  int realPushback;
                  if (hitter->faceRight) {
                    realPushback = -hitBox->pushback;
                    if (hurter->currentState->stateNum == hurter->specialStateMap[SS_PUSH_BLOCK] 
                        || hurter->currentState->stateNum == hurter->specialStateMap[SS_CROUCH_PUSH_BLOCK] ) {
                      realPushback -= 10;
                    }
                    hurter->pushBackVelocity = realPushback;
                  } else {
                    realPushback = hitBox->pushback;
                    if (hurter->currentState->stateNum == hurter->specialStateMap[SS_PUSH_BLOCK] 
                        || hurter->currentState->stateNum == hurter->specialStateMap[SS_CROUCH_PUSH_BLOCK]) {
                    realPushback += 10;
                    }
                    hurter->pushBackVelocity = realPushback;
                  }
                }

                int xEdge = hurter->faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                int visualID = hitBox->guardsparkID;
                VisualEffect& visFX = hurter->guardSparks.at(visualID);
                visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                visFX.setActive(true);
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
                hurter->currentState->counterHitFlag = false;
                if (wasACounter) {
                  hurter->isRed = true;
                  Mix_PlayChannel(0, countah, 0);
                }
                int xEdge = hitter->faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                int visualID = hitBox->hitsparkID;
                VisualEffect& visFX = hitter->hitSparks.at(visualID);
                visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                visFX.setActive(true);

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
                if (hitBox->hitType == GROUND_BOUNCE){
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
                      hurter->hitPushTime = hitBox->airHitPushTime > 0 ? hitBox->airHitPushTime : hitBox->hitPushTime;
                      if (hitter->faceRight) {
                        hurter->hitPushVelX = -hitBox->airHitVelocityX;
                      } else {
                        hurter->hitPushVelX = hitBox->airHitVelocityX;
                      }
                    }
                  }
                  hurter->velocityY = hitBox->hitVelocityY;
                 return {true, wasACounter, hurter->specialStateMap[SS_GROUNDBOUNCE_FLING], NULL};
                } else if(hitBox->hitType == LAUNCHER || hurter->_getYPos() > 0 
                    || airHurtState(hurter->playerNum - 1, hurterCurrentState)){
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
                      hurter->hitPushTime = hitBox->airHitPushTime > 0 ? hitBox->airHitPushTime : hitBox->hitPushTime;
                      if (hitter->faceRight) {
                        hurter->hitPushVelX = -hitBox->airHitVelocityX;
                      } else {
                        hurter->hitPushVelX = hitBox->airHitVelocityX;
                      }
                    }
                  }
                  hurter->velocityY = hitBox->hitVelocityY;
                  return {true, wasACounter, hurter->specialStateMap[SS_AIR_HURT], NULL};
                } else {
                  return {true, wasACounter, hurter->specialStateMap[SS_HURT], NULL};
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
                if (hurter->currentState->stateNum == hurter->specialStateMap[SS_WALK_B]) {
                  hurter->changeState(hurter->specialStateMap[SS_BLOCK_STAND]);
                }
                if (hurter->currentState->stateNum == hurter->specialStateMap[SS_CROUCH] && hurter->_getInput(1)) {
                  hurter->changeState(hurter->specialStateMap[SS_BLOCK_CROUCH]);
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
      p1CounterHit.setStateTime(0);
      p1CounterHit.setActive(true);
    }
  }
  if (p2HitState.hit) {
    player2->changeState(p2HitState.hitState);
    if (p2HitState.counter) {
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
                if((blockState(1, p2StateNum)) || (p2->control && checkBlock(entityHitbox->blockType, p2))){
                  p2->control = 0;
                  bool instantBlocked = p2->_checkCommand(11);
                  if (instantBlocked) {
                    p2->isLight = true;
                    Mix_PlayChannel(0, instantBlock, 0);
                    int realBlockstun = entityHitbox->blockstun - 4;
                    realBlockstun = realBlockstun <= 0 ? 1 : realBlockstun;
                    p2->blockstun = realBlockstun;
                  } else {
                    p2->blockstun = entityHitbox->blockstun;
                  }
                  if (p2->_getYPos() > 0) {
                    // TODO: air blocking state
                    p2->changeState(p2->specialStateMap[SS_BLOCK_STAND]);
                  } else {
                    switch (entityHitbox->blockType) {
                      case 1:
                        if (p2->_getInput(1)) {
                          p2->changeState(p2->specialStateMap[SS_BLOCK_CROUCH]);
                        } else {
                          p2->changeState(p2->specialStateMap[SS_BLOCK_STAND]);
                        }
                        break;
                      case 2:
                        p2->changeState(p2->specialStateMap[SS_BLOCK_CROUCH]);
                        break;
                      case 3:
                        p2->changeState(p2->specialStateMap[SS_BLOCK_STAND]);
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
                   
                  int xEdge = p2->faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                  int visualID = entityHitbox->guardsparkID;
                  VisualEffect& visFX = p2->guardSparks.at(visualID);
                  visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                  visFX.setActive(true);

                  p2->soundsEffects.at(entityHitbox->guardSoundID).active = true;
                  p2->soundsEffects.at(entityHitbox->guardSoundID).channel = p2->soundChannel + 2;
                } else {
                  p2->hitPushTime = entityHitbox->hitPushTime;
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
                  p2->currentState->counterHitFlag = false;
                  if (wasACounter) {
                    Mix_PlayChannel(0, countah, 0);
                  }

                  int xEdge = entity.faceRight ? hitsparkIntersect.x + hitsparkIntersect.w : hitsparkIntersect.x;
                  int visualID = entityHitbox->hitsparkID;
                  VisualEffect& visFX = entity.hitSparks.at(visualID);
                  visFX.reset(xEdge, (hitsparkIntersect.y - (hitsparkIntersect.h/2)));
                  visFX.setActive(true);


                  p2->control = 0;
                  p2->health -= entityHitbox->damage;
                  p2->hitstun = entityHitbox->hitstun;
                  p2->comboCounter++;
                  p2->hurtSoundEffects.at(p2->currentHurtSoundID).active = true;
                  entity.soundsEffects.at(entityHitbox->hitSoundID).active = true;
                  entity.soundsEffects.at(entityHitbox->hitSoundID).channel = p1->soundChannel + 2;

                  if(entityHitbox->hitType == LAUNCHER 
                      || p2->_getYPos() > 0 
                      || p2->currentState->stateNum == p2->specialStateMap[SS_AIR_HURT]){
                    p2->velocityY = entityHitbox->hitVelocityY;
                    return {true, false, p2->specialStateMap[SS_AIR_HURT], NULL};
                  } else {
                    return {true, false, p2->specialStateMap[SS_HURT], NULL};
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
    player1->setXPos(camera.lowerBound+player1->width);
    player1->updateCollisionBoxPositions();
  }

  if(player1->getPos().first + player1->width > worldWidth) {
    player1->setXPos(worldWidth - player1->width);
    player1->updateCollisionBoxPositions();
  }
  if (player1->getPos().first + player1->width > camera.upperBound) {
    player1->setXPos(camera.upperBound - player1->width);
    player1->updateCollisionBoxPositions();
  }

  for (auto& entity : player1->entityList) {
    if (entity.active && entity.isFireball) {
      int entityX = entity.getPos().first;
      int entityW = entity.width;
      bool lowerBound = (entityX) < 0;
      bool lowerCamBound = (entityX) < camera.lowerBound;
      bool upperBound = (entityX) > worldWidth;
      bool upperCamBound = (entityX) > camera.upperBound;

      if (lowerBound || lowerCamBound || upperBound || upperCamBound) {
        entity.deactivateEntity();
      }
    }
  }

  if(player2->getPos().first - player2->width < 0) {
    player2->setXPos(0 + player2->width);
    player2->updateCollisionBoxPositions();
  }
  if (player2->getPos().first - player2->width < camera.lowerBound) {
    player2->setXPos(camera.lowerBound + player2->width);
    player2->updateCollisionBoxPositions();
  }

  if(player2->getPos().first + player2->width > worldWidth) {
    player2->setXPos(worldWidth - player2->width);
    player2->updateCollisionBoxPositions();
  }
  if (player2->getPos().first + player2->width > camera.upperBound) {
    player2->setXPos(camera.upperBound - player2->width);
    player2->updateCollisionBoxPositions();
  }
  for (auto& entity : player2->entityList) {
    if (entity.active && entity.isFireball) {
      int entityX = entity.getPos().first;
      int entityW = entity.width;
      bool lowerBound = (entityX) < 0;
      bool lowerCamBound = (entityX) < camera.lowerBound;
      bool upperBound = (entityX) > worldWidth;
      bool upperCamBound = (entityX) > camera.upperBound;

      if (lowerBound || lowerCamBound || upperBound || upperCamBound) {
        entity.deactivateEntity();
      }
    }
  }
}

void FightState::checkHealth(){

  if (player1->comeback > player1->maxComeback) {
    player1->comeback = player1->maxComeback;
  }
  if (player2->comeback > player2->maxComeback) {
    player2->comeback = player2->maxComeback;
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
  if (player1->comeback < 0 ) {
    player1->comeback = 0;
  }
  if (player2->comeback < 0 ) {
    player2->comeback = 0;
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
      roundWinner = 2;
    }
    if (player2->health <= 0 && player2->hitstun >= 1) {
      player2->isDead = true;
      p1RoundsWon++;
      roundWinner = 1;
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
        p1RoundsWon = 0;
        p2RoundsWon = 0;
        restartRound();
      } else if (p2RoundsWon == 2){
        p1RoundsWon = 0;
        p2RoundsWon = 0;
        restartRound();
      } else {
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

  player1->setXPos(p1StartPos);
  player1->setYPos(0);

  player2->setXPos(p2StartPos);
  player2->setYPos(0);
  updateCamera();
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

void FightState::drawHealthBars(){
  int p1Hp = player1->health;
  float p1HpPercent = (float)p1Hp / (float)player1->maxHealth;
  float p1RedPercent = (float)player1->redHealth/(float)player1->maxHealth;

  int p2Hp = player2->health;
  float p2HpPercent = (float)p2Hp / (float)player2->maxHealth;
  float p2RedPercent = (float)player2->redHealth/(float)player2->maxHealth;

  currentScreen.drawHealthBar(p1HpPercent, p1RedPercent, true);
  currentScreen.drawHealthBar(p2HpPercent, p2RedPercent, false);
  // render meters
  int p1Meter = player1->meter;
  float p1MeterPercent = (float)p1Meter / (float)player1->maxMeter;
  int p2Meter = player2->meter;
  float p2MeterPercent = (float)p2Meter / (float)player2->maxMeter;

  float p1ComebackPercent = (float)player1->comeback / (float)player1->maxComeback;
  float p2ComebackPercent = (float)player2->comeback/ (float)player2->maxComeback;

  currentScreen.drawMeterBar(p1MeterPercent, p1ComebackPercent, true);
  currentScreen.drawMeterBar(p2MeterPercent, p2ComebackPercent, false);
}

void FightState::drawComboCount(){
  int p1ComboCount = player1->comboCounter;
  int p2ComboCount = player2->comboCounter;
  if(p2ComboCount > 1){
    currentScreen.drawComboCount(true, p2ComboCount);
  }
  if(p1ComboCount > 1){
    currentScreen.drawComboCount(false, p1ComboCount);
  }
}

void FightState::drawInputHistory(){
  currentScreen.drawInputHistory(true, player1->virtualController->inputEventList);
  currentScreen.drawInputHistory(false, player2->virtualController->inputEventList);
}

bool FightState::beginGame(const char* name){
  printf("GGPO BEGIN GAME CALLBACK!\n");
  return true;
}

void FightState::ggpoInit(){
  // setup players
  int pNum = stateManager->getPnum();
  p1.player_num = 1;
  p1.size = sizeof(p1);

  p2.player_num = 2;
  p2.size = sizeof(p2);
  int localPort;
  const char* localIp = "127.0.0.1";

  if (pNum == 1) {
    p1.type = GGPO_PLAYERTYPE_LOCAL;
    local_player_handle = &player_handles[0];
    localPort = 7000;

    p2.type = GGPO_PLAYERTYPE_REMOTE;
    strcpy(p2.u.remote.ip_address, localIp);
    p2.u.remote.port = 7001;
  } else {
    p2.type = GGPO_PLAYERTYPE_LOCAL;
    local_player_handle = &player_handles[1];
    localPort = 7001;

    p1.type = GGPO_PLAYERTYPE_REMOTE;
    strcpy(p1.u.remote.ip_address, localIp);
    p1.u.remote.port = 7000;
  }

  // setup GGPO

  ggpoFightState = this;
  GGPOSessionCallbacks cb;
  cb.on_event = fsOnEvent;
  cb.begin_game = fsBeginGame; 
  cb.advance_frame = fsAdvanceFrame;
  cb.load_game_state = fsLoadGameState;
  cb.save_game_state = fsSaveGameState;
  cb.free_buffer = fsFreeBuffer;

  // Start Session
   GGPOErrorCode result = GGPO_OK;
#if defined(SYNC_TEST)
   result = ggpo_start_synctest(&ggpo, &cb, "beatdown", 2, sizeof(int), 1);
#else
  result = ggpo_start_session(&ggpo, &cb, "beatdown", 2, sizeof(int), localPort);
#endif
  ggpo_set_disconnect_timeout(ggpo, 3000);
  ggpo_set_disconnect_notify_start(ggpo, 2000);
  printf("player:%d %s:%d the result of starting GGPO SESSION %d\n", pNum, localIp, localPort, result);

  // Add Player 1
  result = ggpo_add_player(ggpo, &p1, &player_handles[0]);
  printf("player1 type %d, result of add:%d\n", p1.type, result);
  result = ggpo_add_player(ggpo, &p2, &player_handles[1]);
  printf("player2 type %d, result of add:%d\n", p2.type, result);
  // result = ggpo_set_frame_delay(ggpo, *local_player_handle, 3);
  printf("result of add delay:%d\n", result);
}

void FightState::netPlayHandleInput(){
  shouldUpdate = false;
  int inputs[2] = { 0 };
  int currentInput = 0;
  int disconnectFlags;

  VirtualController* p1Vc = player1->virtualController;
  VirtualController* p2Vc = player2->virtualController;
  VirtualController* currentVc = pnum == 1 ? p1Vc : p2Vc;

  currentInput = currentVc->getState();

  printf("adding local input\n");

#if defined(SYNC_TEST)
     currentInput = rand(); // test: use random inputs to demonstrate sync testing
#endif

  GGPOErrorCode result = ggpo_add_local_input(ggpo, *local_player_handle, &currentInput, sizeof(currentInput));
  inputs[0] = currentInput;
  if (GGPO_SUCCEEDED(result)) {
    ggpo_synchronize_input(ggpo, (void *)inputs, sizeof(int)*2, &disconnectFlags);
    if (GGPO_SUCCEEDED(result)) {
      printf("GGPO SYNC SUCCESS\n");
      currentVc->inputHistory.front().clear();
      // simulate a local keypress with input
      p1Vc->setState(inputs[0]);
      p1Vc->addNetInput();
      // p1Vc->inputHistory.front().emplace_back(InputEvent(inputs[0], true));
      // p1Vc->inputEventList.push_back(InputEvent(inputs[0], true));

      p2Vc->setState(inputs[1]);
      p2Vc->addNetInput();
      // p2Vc->inputEventList.push_back(InputEvent(inputs[1], true));
      // p2Vc->inputHistory.front().emplace_back(InputEvent(inputs[1], true));
      shouldUpdate = true;
    }
  } else {
    printf("GGPO SYNC FAIL %d\n", result);
  }
};
