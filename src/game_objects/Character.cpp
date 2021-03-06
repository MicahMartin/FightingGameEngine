#include "game_objects/Character.h"
#include "game_objects/Entity.h"
#include "util/Util.h"
#include <fstream>
#include <sstream>
#include <iostream>



std::string p1InputHistory;
std::string p2InputHistory;

Character::Character(std::pair<int, int> _position, int _playerNum) {
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  maxHealth = 100;
  velocityX = 0;
  velocityY = 0;
  position = _position;
  playerNum = _playerNum;
}

Character::Character(std::pair<int, int> _position) { 
  faceRight = playerNum == 1 ? true : false;
  health = 100;
  maxHealth = 100;
  position = _position;
}

void Character::init(const char* path){
  virtualMachine.character = this;
  stateList.reserve(256);
  entityList.reserve(12);
  loadStates(path);
  printf("stateList size:%ld\n", stateList.size());
  printf("stateList after custom size:%ld\n", stateList.size());
  for (auto i : specialStateMap) {
    printf("SPECIAL STATE MAP %d:%d\n", i.first, i.second);
    
  }
  changeState(specialStateMap[SS_PRE_MATCH]);
}

CharStateObj* Character::saveState(){
  stateObj.virtualControllerObj = *virtualController->saveState();

  stateObj.positionX = position.first;
  stateObj.positionY = position.second;
  stateObj.control = control;
  stateObj.hitstun = hitstun;
  stateObj.blockstun = blockstun;
  stateObj.hitStop = hitStop;
  stateObj.pushTime = pushTime;
  stateObj.pushBackVelocity = pushBackVelocity;
  stateObj.comebackCounter = comebackCounter;
  stateObj.hasAirAction = hasAirAction;
  stateObj.comboCounter = comboCounter;
  stateObj.noGravityCounter = noGravityCounter;
  stateObj.velocityX = velocityX;
  stateObj.velocityY = velocityY;
  stateObj.health = health;
  stateObj.redHealth = redHealth;
  stateObj.redHealthCounter = redHealthCounter;
  stateObj.gravityVal = gravityVal;
  stateObj.hitPushTime = hitPushTime;
  stateObj.hitPushVelX = hitPushVelX;
  stateObj.hitPushVelY = hitPushVelY;
  stateObj.meter = meter;
  stateObj.comeback = comeback;
  stateObj.frameLastAttackConnected = frameLastAttackConnected;
  stateObj.inCorner = inCorner;
  stateObj.inHitStop = inHitStop;
  stateObj.gravity = gravity;
  stateObj.isDead = isDead;
  stateObj.faceRight = faceRight;
  stateObj.inputFaceRight = inputFaceRight;
  stateObj.currentHurtSoundID = currentHurtSoundID;
  stateObj.soundChannel = soundChannel;
  stateObj.flashCounter = flashCounter;
  stateObj.isRed = isRed;
  stateObj.isGreen = isGreen;
  stateObj.isLight = isLight;
  stateObj.installMode = installMode;
  stateObj.auraActive = auraActive;
  stateObj.auraID = auraID;

  stateObj.cancelPointer = cancelPointer;
  stateObj.currentState = currentState->stateNum;
  stateObj.stateDefObj = *currentState->saveState();

  // bool log = playerNum == 1;
  // virtualController->serializeHistory(log);

  // printf("char:%d input history size:%d\n", playerNum, virtualController->inputHistory.size());
  // {
  //   boost::iostreams::basic_array_sink<char> sr(stateObj.inputBuffer, sizeof(stateObj.inputBuffer));
  //   boost::iostreams::stream< boost::iostreams::basic_array_sink<char>> source(sr);
  //   boost::archive::binary_oarchive oa(source);

  //   oa << virtualController->inputHistory;
  //   source.flush();
  //   if(strlen(stateObj.inputBuffer) == 0) {
  //   printf("char:%d save buffer is empty for some reason!!?\n", playerNum);
  //   } else {
  //     printf("char:%d save buffer:%d\n", playerNum, stateObj.inputBuffer);
  //   }

  // }

  // for (int i = 0; i < entityList.size(); ++i) {
  //   stateObj.entityStates[i] = entityList[i].saveState();
  // }
  return &stateObj;
}

void Character::loadState(CharStateObj stateObj){
  printf("GGPO PLAYER:%d { STATE TIME:%d | LOAD STATE:%d | CANCEL POINTER:%d }\n", 
      playerNum, 
      stateObj.stateDefObj.stateTime, 
      stateObj.currentState, 
      stateObj.cancelPointer);

  virtualController->loadState(stateObj.virtualControllerObj);
  // virtualController->inputHistory = stateObj.inputBuffer;

  position.first = stateObj.positionX;
  position.second = stateObj.positionY;
  control = stateObj.control;
  hitstun = stateObj.hitstun;
  blockstun = stateObj.blockstun;
  hitStop = stateObj.hitStop;
  pushTime = stateObj.pushTime;
  pushBackVelocity = stateObj.pushBackVelocity;
  comebackCounter = stateObj.comebackCounter;
  hasAirAction = stateObj.hasAirAction;
  comboCounter = stateObj.comboCounter;
  noGravityCounter = stateObj.noGravityCounter;
  velocityX = stateObj.velocityX;
  velocityY = stateObj.velocityY;
  health = stateObj.health;
  redHealth = stateObj.redHealth;
  redHealthCounter = stateObj.redHealthCounter;
  gravityVal = stateObj.gravityVal;
  hitPushTime = stateObj.hitPushTime;
  hitPushVelX = stateObj.hitPushVelX;
  hitPushVelY = stateObj.hitPushVelY;
  meter = stateObj.meter;
  comeback = stateObj.comeback;
  frameLastAttackConnected = stateObj.frameLastAttackConnected;
  inCorner = stateObj.inCorner;
  inHitStop = stateObj.inHitStop;
  gravity = stateObj.gravity;
  isDead = stateObj.isDead;
  faceRight = stateObj.faceRight;
  inputFaceRight = stateObj.inputFaceRight;
  currentHurtSoundID = stateObj.currentHurtSoundID;
  soundChannel = stateObj.soundChannel;
  flashCounter = stateObj.flashCounter;
  isRed = stateObj.isRed;
  isGreen = stateObj.isGreen;
  isLight = stateObj.isLight;
  installMode = stateObj.installMode;
  auraActive = stateObj.auraActive;
  auraID = stateObj.auraID;

  cancelPointer = stateObj.cancelPointer;
  setCurrentState(stateObj.currentState);
  currentState->loadState(stateObj.stateDefObj);

  // virtualController->inputHistorySnapShot = stateObj.inputHistory;
  // bool log = playerNum == 1;
  // virtualController->loadHistory(virtualController->inputHistorySnapShot, log);
  
  // for (int i = 0; i < entityList.size(); ++i) {
  //   entityList[i].loadState(stateObj.entityStates[i]);
  // }
  // {
  //   boost::iostreams::basic_array_source<char> device(stateObj.inputBuffer, sizeof(stateObj.inputBuffer));
  //   boost::iostreams::stream<boost::iostreams::basic_array_source<char>> s(device);
  //   boost::archive::binary_iarchive ia(s);
  //   ia >> virtualController->inputHistory;
  // }
}

void Character::refresh(){
  changeState(specialStateMap[SS_PRE_MATCH]);
  health = 100;
  redHealth = 100;
  control = 1;
  hitstun = 0;
  blockstun = 0;
  hitStop = 0;
  pushTime = 0;
  pushBackVelocity = 0;
  hasAirAction = 0;
  comboCounter = 0;
  cancelPointer = 0;
  noGravityCounter = 0;
  frameLastAttackConnected = 0;
  inCorner = false;
  inHitStop = false;
  gravity = true;
  isDead = false;
  velocityX = 0;
  velocityY = 0;
  currentHurtSoundID = 1;
}

void Character::changeState(int stateDefNum){
  for (auto &i : visualEffects) {
    if (i.second.getAura()) {
      i.second.setActive(false);
    }
  }
  auraActive = false;
  cancelPointer = 0;
  if(stateDefNum >= 6000){
    int theNum = stateDefNum - 6000;
    int customStateNum = stateCount + theNum;
    // printf("the num:%d, the customStateNum%d\n", stateDefNum, customStateNum);
    currentState = &stateList.at(customStateNum-1);
  } else if (stateDefNum >= 5000) {
    int theNum = stateDefNum - 5000;
    SpecialState theState = (SpecialState)theNum;
    int specialStateNum = specialStateMap[theState];
    // printf("the num:%d, the specialStateNum %d\n", stateDefNum, specialStateNum);
    currentState = &stateList.at(specialStateNum-1);
  } else {
    currentState = &stateList.at(stateDefNum-1);
  }

  if(!currentState->checkFlag(NO_TURN_ON_ENTER)){
    updateFaceRight();
  }
  currentState->enter();
  updateCollisionBoxPositions();
  updateCollisionBoxes();
};

void Character::setCurrentState(int stateDefNum){
  if(stateDefNum >= 6000){
    int theNum = stateDefNum - 6000;
    int customStateNum = stateCount + theNum;
    // printf("the num:%d, the customStateNum%d\n", stateDefNum, customStateNum);
    currentState = &stateList.at(customStateNum-1);
  } else if (stateDefNum >= 5000) {
    int theNum = stateDefNum - 5000;
    SpecialState theState = (SpecialState)theNum;
    int specialStateNum = specialStateMap[theState];
    // printf("the num:%d, the specialStateNum %d\n", stateDefNum, specialStateNum);
    currentState = &stateList.at(specialStateNum-1);
  } else {
    // printf("the the stateNum:%d\n", stateDefNum);
    currentState = &stateList.at(stateDefNum-1);
  }
}

void Character::cancelState(int stateDefNum){
  // printf("oh yeah cancel that bitch %d\n", stateDefNum);
  if (stateDefNum >= 6000) {
    int theNum = stateDefNum - 6000;
    int customState = stateCount + theNum;
    // printf("the num:%d, the customState%d\n", theNum, customState);
    cancelPointer = customState;
  } else if (stateDefNum >= 5000) {
    int theNum = stateDefNum - 5000;
    SpecialState theState = (SpecialState)theNum;
    int specialStateNum = specialStateMap[theState];
    cancelPointer = specialStateNum;
  } else {
    cancelPointer = stateDefNum;
  }
};

void Character::loadStates(const char* path){
  // printf("%d Loading states\n", playerNum);
  std::ifstream configFile(path);
  configFile >> stateJson;
  // compile inputs
  if (stateJson.count("anim_scale")) {
    std::string animScaleStr = stateJson.at("anim_scale");
    // printf("the animScale %s\n", animScaleStr.c_str());
    animScale = std::stod(animScaleStr);
  }
  if (stateJson.count("gravity")) {
    std::string grav = stateJson.at("gravity");
    printf("the grav %s\n", grav.c_str());
    gravityVal = std::stod(grav);
  }
  if (stateJson.count("velocityMaximumX")) {
  }
  if (stateJson.count("velocityMinimumX")) {
  }
  if (stateJson.count("velocityMaximumY")) {
  }
  if (stateJson.count("velocityMinimumY")) {
    velocityMinimumY = stateJson.at("velocityMinimumY");
  }

  std::string str = stateJson.at("command_script").get<std::string>();
  const char* ptr = str.c_str();
  if(!virtualMachine.compiler.compile(ptr, &inputScript, "input script")){
    inputScript.disassembleScript("input command script");
    throw std::runtime_error("inputScript failed to compile");
  }

  // load states
  printf("loading states\n");
  for(auto i : stateJson.at("states").items()){
    StateDef* newStateDef = &stateList.emplace_back();
    newStateDef->owner = this;
    newStateDef->charName = charName;
    newStateDef->init(i.value(), &virtualMachine, animScale);
  }

  printf("loading entities\n");
  entityList.reserve(12);
  for(auto i : stateJson.at("entities").items()){
    if (i.value().count("defPath")) {
      std::string defString = i.value().at("defPath");
      printf("the defPath %s\n", defString.c_str());
      entityList.emplace_back(this, i.value().at("entityID"), defString.c_str()).init();
      printf("done loading entities\n");
    }
  }

  for(auto i : stateJson.at("animation_assets").items()){
    int visualID = i.value().at("assetID");
    visualEffects.emplace(visualID, VisualEffect{}).first->second.anim.loadAnimEvents(i.value().at("animation"));
    visualEffects.at(visualID).setPlayLength(visualEffects.at(visualID).anim.animationTime);
    if (i.value().count("aura")) {
      visualEffects.at(visualID).setAura(i.value().at("aura"));
    }
    if (i.value().count("charEffect")) {
      visualEffects.at(visualID).charEffect = (i.value().at("charEffect"));
    }
    printf("loaded visual effect # %d\n", visualID);
  }

  for(auto i : stateJson.at("hit_sparks").items()){
    int visualID = i.value().at("assetID");
    hitSparks.emplace(visualID, VisualEffect{}).first->second.anim.loadAnimEvents(i.value().at("animation"));
    hitSparks.at(visualID).setPlayLength(hitSparks.at(visualID).anim.animationTime);
    printf("loaded hitspark effect #%d\n", visualID);
  }

  for(auto i : stateJson.at("guard_sparks").items()){
    int visualID = i.value().at("assetID");
    guardSparks.emplace(visualID, VisualEffect{}).first->second.anim.loadAnimEvents(i.value().at("animation"));
    guardSparks.at(visualID).setPlayLength(guardSparks.at(visualID).anim.animationTime);
    printf("loaded guardpark effect #%d\n", visualID);
  }

  for(auto i : stateJson.at("audio_assets").items()){
    int soundID = i.value().at("assetID");

    std::string path(i.value().at("file").get<std::string>());
    const char* pathPointer = path.c_str();

    Mix_Chunk* soundEffectPointer = Mix_LoadWAV(pathPointer);
    soundsEffects.emplace(soundID, SoundObj{soundEffectPointer, false, 0});
  }
  for(auto i : stateJson.at("hurt_sounds").items()){
    int hurtSoundID = i.value().at("assetID");

    std::string path(i.value().at("file").get<std::string>());
    const char* pathPointer = path.c_str();

    Mix_Chunk* hurtSoundPointer = Mix_LoadWAV(pathPointer);
    hurtSoundEffects.emplace(hurtSoundID, SoundObj{hurtSoundPointer, false, 0, soundChannel + 1});
  }
  // loadSpecialStates
  {
    auto i = stateJson.at("special_state_nums");
    specialStateMap[SS_IDLE] = i.at("SS_IDLE");
    specialStateMap[SS_WALK_F] = i.at("SS_WALK_F");
    specialStateMap[SS_WALK_B] = i.at("SS_WALK_B");
    specialStateMap[SS_CROUCH] = i.at("SS_CROUCH");
    specialStateMap[SS_JUMP_N] = i.at("SS_JUMP_N");
    specialStateMap[SS_JUMP_F] = i.at("SS_JUMP_F");
    specialStateMap[SS_JUMP_B] = i.at("SS_JUMP_B");
    specialStateMap[SS_HURT] = i.at("SS_HURT");
    specialStateMap[SS_HURT_RECOVERY] = i.at("SS_HURT_RECOVERY");
    specialStateMap[SS_AIR_HURT] = i.at("SS_AIR_HURT");
    specialStateMap[SS_AIR_HURT_RECOVERY] = i.at("SS_AIR_HURT_RECOVERY");
    specialStateMap[SS_KNOCKDOWN] = i.at("SS_KNOCKDOWN");
    specialStateMap[SS_BLOCK_STAND] = i.at("SS_BLOCK_STAND");
    specialStateMap[SS_BLOCK_CROUCH] = i.at("SS_BLOCK_CROUCH");
    specialStateMap[SS_BLOWBACK_FALLING] = i.at("SS_BLOWBACK_FALLING");
    specialStateMap[SS_AIR_BLOCK] = i.at("SS_AIR_BLOCK");
    specialStateMap[SS_PRE_MATCH] = i.at("SS_PRE_MATCH");
    specialStateMap[SS_DEAD_STANDING] = i.at("SS_DEAD_STANDING");
    specialStateMap[SS_DEAD_KNOCKDOWN] = i.at("SS_DEAD_KNOCKDOWN");
    specialStateMap[SS_KNOCKDOWN] = i.at("SS_KNOCKDOWN");
    specialStateMap[SS_THROW_TECH] = i.at("SS_THROW_TECH");
    specialStateMap[SS_AIR_THROW_TECH] = i.at("SS_AIR_THROW_TECH");
    specialStateMap[SS_PUSH_BLOCK] = i.at("SS_PUSH_BLOCK");
    specialStateMap[SS_CROUCH_PUSH_BLOCK] = i.at("SS_CROUCH_PUSH_BLOCK");
    specialStateMap[SS_GROUNDBOUNCE_FLING] = i.at("SS_GROUNDBOUNCE_FLING");
    specialStateMap[SS_GROUNDBOUNCE_IMPACT] = i.at("SS_GROUNDBOUNCE_IMPACT");
    specialStateMap[SS_JUMP_R] = i.at("SS_JUMP_R");
    specialStateMap[SS_AIR_TECH] = i.at("SS_AIR_TECH");
    specialStateMap[SS_DEAD_FALLING] = i.at("SS_DEAD_FALLING");
    specialStateMap[SS_FLOAT_HURT] = i.at("SS_FLOAT_HURT");
    specialStateMap[SS_FLOAT_HURT_RECOVERY] = i.at("SS_FLOAT_HURT_RECOVERY");
    specialStateMap[SS_FORWARD_THROW] = i.at("SS_FORWARD_THROW");
    specialStateMap[SS_FORWARD_THROW_ATTEMPT] = i.at("SS_FORWARD_THROW_ATTEMPT");
    specialStateMap[SS_FORWARD_THROW_SUCCESS] = i.at("SS_FORWARD_THROW_SUCCESS");
    specialStateMap[SS_GROUND_THROW_TECH] = i.at("SS_GROUND_THROW_TECH");
    specialStateMap[SS_BACK_THROW] = i.at("SS_BACK_THROW");
    specialStateMap[SS_BACK_THROW_ATTEMPT] = i.at("SS_BACK_THROW_ATTEMPT");
    specialStateMap[SS_BACK_THROW_SUCCESS] = i.at("SS_BACK_THROW_SUCCESS");
  }


  configFile.close();
  stateCount = stateList.size();
}

void Character::loadCustomStates(const char* path){
  stateJson.clear();
  std::ifstream configFile(path);
  configFile >> stateJson;
  // load states
  printf("loading custom states\n");
  int counter = 0;

  for(auto i : stateJson.at("custom_states").items()){
    StateDef* newStateDef = &stateList.emplace_back();
    newStateDef->owner = this;
    newStateDef->charName = charName;
    newStateDef->init(i.value(), &virtualMachine, animScale);
    ++counter;
  }
  printf("custom state count value:%d\n", counter);

  configFile.close();
  stateJson.clear();
}

Character::~Character(){};

void Character::handleInput(){ 
  if(cancelPointer != 0 && !inHitStop){
    changeState(cancelPointer);
  }

  if(control){
    // TODO: Precompile all scripts
    // printf("char:%d handling input\n", playerNum);
    virtualMachine.execute(&inputScript);
  }
};

void Character::update(){ 
  if (installMode) {
    if (installCounter++ == 1) {
      installCounter = 0;
      comeback -= 3;
    }
  }
  if(pushTime > 0) {
    pushTime--;
    if(pushTime == 0){
      pushBackVelocity = 0;
    }
  }
  if(hitPushTime > 0) {
    hitPushTime--;
    if(hitPushTime == 0){
      hitPushVelX = 0;
    }
  }

  if (hitstun > 0) {
    hitstun--;
  }

  if (blockstun > 0) {
    blockstun--;
  }

  if (comebackCounter-- == 0) {
    comebackCounter = 30;
    if (comeback++ >= maxComeback) {
      comeback = maxComeback;
    }
  }


  if (currentState->visualEffectMap.count(currentState->stateTime)) {
    int visualID = currentState->visualEffectMap.at(currentState->stateTime);
    VisualEffect& visFX = visualEffects.at(visualID);
    // printf("found visFX for frame %d, the playLEngth %d\n", currentState->stateTime, visFX.getPlayLength());
    visFX.reset(position.first, position.second);
    visFX.setActive(true);
  }
  for (auto i : currentState->visualEffectMap) {
    VisualEffect& visFX = visualEffects.at(i.second);
    // printf("found visFX for frame %d, the playLEngth %d\n", currentState->stateTime, visFX.getPlayLength());
    if (visFX.getAura()) {
      visFX.setX(position.first);
      visFX.setY(position.second);
    }
  }
  if (currentState->soundIndexMap[currentState->stateTime].size() > 0) {
    for (int soundID : currentState->soundIndexMap[currentState->stateTime]) {
      soundsEffects.at(soundID).active = true;
      soundsEffects.at(soundID).channel = soundChannel;
    }
  }

  if (isRed) {
    isRed = false;
  }
  if (isGreen) {
    isGreen = false;
  }
  if (isLight) {
    isLight = false;
  }
  if (installMode) {
    if(flashCounter++ < 8){
      isRed = true;
    } else if(flashCounter < 16){
      isRed = false;
    }
    if(flashCounter == 16){
      flashCounter = 0;
    }
  }

  currentState->update();

  updatePosition();
  updateCollisionBoxPositions();
  updateCollisionBoxes();
};

void Character::updateFaceRight(){
  if (position.first == otherChar->getPos().first) {
  } else {
    if(position.first < otherChar->getPos().first){
      faceRight = true;
      inputFaceRight = true;
    } else {
      faceRight = false;
      inputFaceRight = false;
    }
  }
};

void Character::activateVisFX(int visID){
  VisualEffect& visFX = visualEffects.at(visID);
  visFX.reset(position.first, position.second);
  visFX.setActive(true);
}

void Character::updatePosition() {
  // _negVelSetX(pushBackVelocity);
  int velX = velocityX;
  int stateNum = currentState->stateNum;
  bool inHurtState = stateNum == specialStateMap[SS_HURT]
    || stateNum == specialStateMap[SS_AIR_HURT]
    || stateNum == specialStateMap[SS_BLOWBACK_FALLING]
    || stateNum == specialStateMap[SS_GROUNDBOUNCE_FLING]
    || stateNum == specialStateMap[SS_GROUNDBOUNCE_IMPACT];
  if (hitstun > 0 || inHurtState) {
    velX = velocityX - hitPushVelX;
  } else {
    velX = velocityX - pushBackVelocity;
  }
  position.first += velX;
  position.second -= velocityY;

  if(noGravityCounter > 0){
    gravity = false;
    if(--noGravityCounter <= 0){
      noGravityCounter = 0;
      gravity = true;
    }
  }

  if(position.second < 0 && gravity){
    velocityY -= gravityVal;
    if ((velocityMinimumY != 0) && velocityY < velocityMinimumY) {
      velocityY = velocityMinimumY;
    }
  }

  if(position.second > 0){
    position.second = 0;
    velocityY = 0;
  }
}

void Character::updateCollisionBoxPositions(){
  for (auto cb : currentState->pushBoxes) {
    cb->positionX = position.first - (cb->width/2);
    cb->positionY = position.second - cb->offsetY;
  }

  for (auto cb : currentState->hurtBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
  }

  for (auto cb : currentState->hitBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
  }

  for (auto cb : currentState->throwHitBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
  }

  for (auto cb : currentState->throwHurtBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
  }

  for (auto cb : currentState->proximityBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
  }
  for (auto cb : currentState->projectileBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
  }
}

void Character::updateCollisionBoxes(){
  // TODO: abstract into updateCollisionBoxPos function
  int stateTime = currentState->stateTime + 1;
  for (auto cb : currentState->pushBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if ((cb->end == -1 && (stateTime > cb->start)) || stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->hurtBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if ((cb->end == -1 && (stateTime > cb->start)) || stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->throwHurtBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (cb->end == -1 || stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->hitBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->throwHitBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }

  for (auto cb : currentState->proximityBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }
  for (auto cb : currentState->projectileBoxes) {
    if (stateTime < cb->start) {
      cb->disabled = true;
    }
    if (stateTime == cb->start) {
      cb->disabled = false;
    }
    if (stateTime == cb->end) {
      cb->disabled = true;
    }
  }
}

void Character::draw(){
  bool fakeHitstop = (inHitStop && (hitstun > 0 || blockstun > 0 || currentState->stateNum == specialStateMap[SS_THROW_TECH]));
  currentState->anim.isRed = isRed;
  currentState->anim.isLight = isLight;
  currentState->anim.isGreen = isGreen;
  currentState->draw(position, faceRight, fakeHitstop);

  // SDL_Renderer* renderer = Graphics::getInstance()->getRenderer();
  // int windowHeight = Graphics::getInstance()->getWindowHeight();
  // int camOffset = Graphics::getInstance()->getCamera()->cameraRect.x;

  // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
  // SDL_RenderDrawLine(renderer, (position.first/COORDINATE_SCALE) - camOffset, windowHeight, (position.first/COORDINATE_SCALE) - camOffset, (position.second/COORDINATE_SCALE));
  // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
};

void Character::drawEntities(){
  for (auto &i : entityList) {
    if (i.active) {
      i.draw();
      for (auto &v : i.visualEffects) {
        if (v.second.getActive()) {
          v.second.draw(i.faceRight);
        }
      }
      for (auto &s : i.hitSparks) {
        if (s.second.getActive()) {
          s.second.draw(i.faceRight);
        }
      }
      
    }
  }
}

void Character::drawFX(){
  for (auto &i : visualEffects) {
    VisualEffect& visFX = i.second;
    if (visFX.getActive()) {
      visFX.draw(faceRight);
    }
  }
  for (auto &i : guardSparks) {
    VisualEffect& visFX = i.second;
    if (visFX.getActive()) {
      visFX.draw(faceRight);
    }
  }
  for (auto &i : hitSparks) {
    VisualEffect& visFX = i.second;
    if (visFX.getActive()) {
      visFX.draw(faceRight);
    }
  }
}

std::pair<int,int> Character::getPos(){
  return position;
};

StateDef* Character::getCurrentState(){
  return currentState;
};

Mix_Chunk* Character::getSoundWithId(int id){
  return soundsEffects.at(id).sound;
};

int Character::getSoundChannel(){
  return soundChannel;
};

int Character::getAnimScale(){
  return animScale;
};

void Character::setXPos(int x){
  position.first = x;
};

void Character::setYPos(int y){
  position.second = y;
};

void Character::setX(int x){
  position.first += x;
};

void Character::setY(int y){
  position.second -= y;
};

bool Character::blockState(int state){
return ( state == specialStateMap[SS_BLOCK_STAND]
          || state == specialStateMap[SS_BLOCK_CROUCH]
          || state == specialStateMap[SS_AIR_BLOCK]
          || state == specialStateMap[SS_PUSH_BLOCK]
          || state == specialStateMap[SS_CROUCH_PUSH_BLOCK]
          || state == specialStateMap[SS_AIR_PUSH_BLOCK]
      );
}  

bool Character::hurtState(int state){
  return (
      state == specialStateMap[SS_HURT]
      || state == specialStateMap[SS_HURT_RECOVERY]
      || state == specialStateMap[SS_AIR_HURT]
      || state == specialStateMap[SS_AIR_HURT_RECOVERY]
      || state == specialStateMap[SS_BLOWBACK_FALLING]
      || state == specialStateMap[SS_DEAD_STANDING]
      || state == specialStateMap[SS_DEAD_KNOCKDOWN]
      || state == specialStateMap[SS_GROUNDBOUNCE_FLING]
      || state == specialStateMap[SS_GROUNDBOUNCE_IMPACT]);
}

void Character::setFlag(ObjFlag flag){

  flags |= flag;
}

void Character::clearFlag(ObjFlag flag){

  flags &= (~flag);
}

bool Character::getFlag(ObjFlag flag){

  return (( flags & flag ) == flag);
}


void Character::_changeState(int  stateNum){
  changeState(stateNum);
}

void Character::_cancelState(int  stateNum){
  cancelState(stateNum);
}

void Character::_velSetX(int ammount){
 velocityX = faceRight ? ammount : -ammount;
 // momentum = (mass/100) * velocityX;
}

void Character::_negVelSetX(int ammount){
 velocityX = faceRight ? -ammount : ammount;
 // momentum = (mass/100) * velocityX;
}

void Character::_velSetY(int ammount){
  velocityY = ammount;
}

void Character::_moveForward(int ammount){
  velocityX += faceRight ? ammount : -ammount;
}

void Character::_moveBack(int ammount){
  faceRight ? setX(-ammount) : setX(ammount);
}

void Character::_moveUp(int ammount){
  setY(ammount);
}

void Character::_moveDown(int ammount){
  setY(-ammount);
}

void Character::_setControl(int val){
  control = val;
}

void Character::_setCombo(int val){
  comboCounter = val;
}

void Character::_setGravity(int on){
  gravity = on;
}

void Character::_setNoGravityCounter(int count){
  noGravityCounter += count;
}

void Character::_resetAnim(){
  currentState->resetAnim();
}

void Character::_activateEntity(int entityID){
  entityList[entityID - 1].activateEntity();
}

void Character::_deactivateEntity(int entityID){
  printf("deactivating entity:%d \n", entityID);
  entityList[entityID - 1].deactivateEntity();
}

void Character::_snapToOpponent(int offset){
  auto opponentPos = otherChar->getPos();
  bool opponentFaceRight = otherChar->faceRight;

  position.first = opponentFaceRight ? (opponentPos.first + offset) : (opponentPos.first - offset);
  position.second = opponentPos.second;
  printf("snapping to opponentPos:%d:%d, newPos:%d:%d\n", opponentPos.first, opponentPos.second, position.first, position.second);
}


int Character::_getAnimTime(){
  return currentState->anim.timeRemaining();
}

int Character::_getYPos(){
  int yPos = abs(getPos().second);
  return yPos;
}

int Character::_getStateTime(){
  return currentState->stateTime;
}

int Character::_getInput(int input){
  Input inputType = VirtualController::inputMap[input](inputFaceRight);
  return virtualController->isPressed(inputType) ? 1 : 0;
}

int Character::_getStateNum(){
  return currentState->stateNum;
}

int Character::_getControl(){
  return control;
}

int Character::_getAirActions(){
  return hasAirAction;
}

void Character::_setHitStun(int operand){
  hitstun = operand;
}

void Character::_setHitCancel(int val){
  currentState->canHitCancel = val;
}

void Character::_setWhiffCancel(int val){
  currentState->canWhiffCancel = val;
}

void Character::_setAirAction(int operand){
  hasAirAction = operand;
}

void Character::_setCounter(int operand){
  currentState->counterHitFlag = operand;
}

int Character::_getCombo(){
  return comboCounter;
}

int Character::_wasPressed(int input){
  return virtualController->wasPressedBuffer(VirtualController::inputMap[input](inputFaceRight)) ? 1 : 0;
}

int Character::_getHitStun(){
  return hitstun;
}

int Character::_getHitCancel(){
  return currentState->canHitCancel;
}

int Character::_getWhiffCancel(){
  return currentState->canWhiffCancel;
}

int Character::_getBlockStun(){
  return blockstun;
}

int Character::_getIsAlive(){
  return !isDead;
}

int Character::_getMeter(int meterIndex){
  return meterArray[meterIndex];
}

int Character::_getComebackMeter(){
  return comeback;
}

void Character::_addMeter(int i){
  std::string meterString = std::to_string(i);
  int meterIndex = meterString.front();
  int meterValue = std::stoi(meterString.substr(1));
  meterArray[meterIndex] += meterValue;
}

void Character::_setMeter(int i){
  std::string meterString = std::to_string(i);
  int meterIndex = meterString.front();
  int meterValue = std::stoi(meterString.substr(1));
  meterArray[meterIndex] = meterValue;
}

void Character::_subtractMeter(int i){
  std::string meterString = std::to_string(i);
  int meterIndex = meterString.front();
  int meterValue = std::stoi(meterString.substr(1));
  meterArray[meterIndex] -= meterValue;
}

int Character::_checkCommand(int commandIndex){
  return virtualController->checkCommand(commandIndex, inputFaceRight);
}

void Character::_setBlockstun(int input){
  blockstun = input;
};

void Character::_setInstall(int input){
  printf("setting installMode to %d\n", input);
  installMode = input;
};

int Character::_getEntityStatus(int entityID){
  printf("is the entity active? %d, %d\n", entityID, entityList[entityID - 1].active);
  return entityList.at(entityID - 1).active;
};

int Character::_getInstall(){
  return installMode;
};

int Character::_getVelY(){
  return velocityY;
};

int Character::_getVelX(){
  return velocityX;
};
