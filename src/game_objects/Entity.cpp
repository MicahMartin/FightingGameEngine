#include <fstream>
#include <utility>
#include "game_objects/Entity.h"
#include "game_objects/Character.h"

Entity::Entity(Character* owner, int entityID, const char* defPath) : owner(owner), entityID(entityID), defPath(defPath) {
  virtualController = owner->virtualController;
  printf("in entity constructor\n");
}
Entity::~Entity(){}

void Entity::init(){
  printf("in entity init\n");
  virtualMachine.character = this;
  stateList.reserve(256);
  loadStates();
  printf("changing state to idle\n");
  changeState(1);
}

void Entity::activateEntity(){
  std::pair<int,int> ownerPos = owner->getPos();

  active = true;
  control = 1;
  hitstun = 0;
  blockstun = 0;
  hitStop = 0;
  pushTime = 0;
  cancelPointer = 0;
  noGravityCounter = 0;
  frameLastAttackConnected = 0;
  inCorner = false;
  inHitStop = false;
  gravity = true;
  health = 1;
  velocityX = 0;
  velocityY = 0;
  faceRight = owner->faceRight;
  inputFaceRight = owner->inputFaceRight;

  // TODO: spawnOffset
  position = {ownerPos.first + spawnOffsetX, ownerPos.second + spawnOffsetY};
  changeState(1);
};

void Entity::deactivateEntity(){
  active = false;
};

void Entity::changeState(int stateDefNum){
  cancelPointer = 0;
  currentState = &stateList.at(stateDefNum-1);
  currentState->enter();
  updateCollisionBoxes();
};

void Entity::cancelState(int stateDefNum){
  cancelPointer = stateDefNum;
};

void Entity::loadStates(){
  printf("entity:%d Loading states from :%s\n", entityID, defPath);
  std::ifstream configFile(defPath);
  configFile >> stateJson;

  if (stateJson.count("anim_scale")) {
    std::string animScaleStr = stateJson.at("anim_scale");
    animScale = std::stod(animScaleStr);
    printf("the animScale %f\n", animScale);
  }
  if (stateJson.count("spawnOffsetX")) {
    spawnOffsetX = stateJson.at("spawnOffsetX");
  }
  if (stateJson.count("isFireball")) {
    isFireball = stateJson.at("isFireball");
  }
  if (stateJson.count("spawnOffsetY")) {
    spawnOffsetY = stateJson.at("spawnOffsetX");
  }
  // compile inputs
  if (stateJson.count("command_script")) {
    std::string str = stateJson.at("command_script").get<std::string>();
    const char* ptr = str.c_str();
    if(!virtualMachine.compiler.compile(ptr, &inputScript, "input script")){
      inputScript.disassembleScript("input command script");
      throw std::runtime_error("inputScript failed to compile");
    }
    hasCommandScript = true;
  }
  if (stateJson.count("update_facing")) {
    updateFacing = stateJson.at("update_facing");
  }
  // load states
  for(auto i : stateJson.at("states").items()){
    StateDef* createdState = &stateList.emplace_back();
    createdState->init(i.value(), &virtualMachine, animScale);
    createdState->owner = this;
  }

  if (stateJson.count("animation_assets")) {
    for(auto i : stateJson.at("animation_assets").items()){
      int visualID = i.value().at("assetID");
      visualEffects.emplace(visualID, VisualEffect{}).first->second.anim.loadAnimEvents(i.value().at("animation"));
      visualEffects.at(visualID).setPlayLength(visualEffects.at(visualID).anim.animationTime);
      printf("loaded visual effect # %d\n", visualID);
    }
  }

  if (stateJson.count("hit_sparks")) {
    printf("loading entity hitsparks\n");
    for(auto i : stateJson.at("hit_sparks").items()){
      int visualID = i.value().at("assetID");
      hitSparks.emplace(visualID, VisualEffect{}).first->second.anim.loadAnimEvents(i.value().at("animation"));
      hitSparks.at(visualID).setPlayLength(hitSparks.at(visualID).anim.animationTime);
    }
  }
  if (stateJson.count("audio_assets")) {
    for(auto i : stateJson.at("audio_assets").items()){
      int soundID = i.value().at("assetID");

      std::string path(i.value().at("file").get<std::string>());
      const char* pathPointer = path.c_str();

      Mix_Chunk* soundEffectPointer = Mix_LoadWAV(pathPointer);
      soundsEffects.emplace(soundID, SoundObj{soundEffectPointer, false, 0});
    }
  }
  printf("done loading entity\n");
  configFile.close();
}

void Entity::handleInput() {
  if (active) {
    if(pushTime > 0){
      pushTime--;
      if(pushTime == 0){
        velocityX = 0;
      }
    }

    if (hitstun > 0) {
      hitstun--;
    }

    if (blockstun > 0) {
      blockstun--;
    }

    if(cancelPointer != 0){
      changeState(cancelPointer);
    }

    if(hasCommandScript && control){
      // TODO: Precompile all scripts
      virtualMachine.execute(&inputScript);
    }
  }
};

void Entity::update(){
  if (active) {
    if (currentState->visualEffectMap.count(currentState->stateTime)) {
      int visualID = currentState->visualEffectMap.at(currentState->stateTime);
      VisualEffect& visFX = visualEffects.at(visualID);
      printf("found visFX for entities frame %d, the playLEngth %d\n", currentState->stateTime, visFX.getPlayLength());
      visFX.reset(position.first, position.second);
      visFX.setActive(true);
    }
    // TODO: Sound method
    if (currentState->soundIndexMap[currentState->stateTime].size() > 0) {
      for (int soundID : currentState->soundIndexMap[currentState->stateTime]) {
        // printf("trying to play soundID %d\n", soundID);
        soundsEffects.at(soundID).active = true;
      }
    }

    currentState->update();
    updatePosition();
    updateCollisionBoxes();
  }
};

void Entity::updateFaceRight(){
};

void Entity::updatePosition(){
  position.first += velocityX;
  position.second -= velocityY;

  if(noGravityCounter > 0){
    gravity = false;
    if(--noGravityCounter == 0){
      gravity = true;
    }
  };
  if(position.second < 0 && gravity){
    velocityY -= gravityVal;
  }

  if(position.second > 0){
    position.second = 0;
    velocityY = 0;
  }
}
void Entity::updateCollisionBoxPositions(){
  for (auto cb : currentState->pushBoxes) {
    cb->positionX = position.first - (cb->width / 2);
    cb->positionY = position.second;
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

void Entity::updateCollisionBoxes(){
  // TODO: abstract into updateCollisionBoxPos function
  int stateTime = currentState->stateTime;
  for (auto cb : currentState->pushBoxes) {
    cb->positionX = position.first - (cb->width / 2);
    cb->positionY = position.second;
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

  for (auto cb : currentState->hurtBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
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

  for (auto cb : currentState->throwHurtBoxes) {
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
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
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
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
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
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
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
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
    cb->positionX = position.first + (faceRight ? cb->offsetX : - (cb->offsetX + cb->width));
    cb->positionY = position.second - cb->offsetY;
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

void Entity::draw(){
  // draw health bars
  // printf("trying to draw an entity..\n");
  if (active) {
    // printf("im active so ima draw!, am I in hitstop tho? %d\n", inHitStop);
    currentState->draw(position, faceRight, inHitStop);
  }
};


EntityStateObj Entity::saveState(){
  EntityStateObj stateObj;

  stateObj.control = control;
  stateObj.hitstun = hitstun;
  stateObj.blockstun = blockstun;
  stateObj.hitStop = hitStop;
  stateObj.pushTime = pushTime;
  stateObj.hasAirAction = hasAirAction;
  stateObj.comboCounter = comboCounter;
  stateObj.cancelPointer = cancelPointer;
  stateObj.noGravityCounter = noGravityCounter;
  stateObj.gravityVal = gravityVal;
  stateObj.health = health;
  stateObj.velocityX = velocityX;
  stateObj.velocityY = velocityY;
  stateObj.frameLastAttackConnected = frameLastAttackConnected;
  stateObj.inCorner = inCorner;
  stateObj.active = active;
  stateObj.inHitStop = inHitStop;
  stateObj.gravity = gravity;
  stateObj.faceRight = faceRight;
  stateObj.inputFaceRight = inputFaceRight;
  stateObj.isDead = isDead;
  stateObj.updateFacing = updateFacing;
  stateObj.currentStateNum = currentState->stateNum;
  stateObj.currentStateObj = currentState->saveState();
  stateObj.positionX = position.first;
  stateObj.positionY = position.second;

  return stateObj;
}

void Entity::loadState(EntityStateObj stateObj){
  position.first = stateObj.positionX;
  position.second = stateObj.positionY;
  control = stateObj.control;
  hitstun =  stateObj.hitstun;
  blockstun = stateObj.blockstun;
  hitStop = stateObj.hitStop;
  pushTime = stateObj.pushTime;
  hasAirAction = stateObj.hasAirAction;
  comboCounter = stateObj.comboCounter;
  cancelPointer = stateObj.cancelPointer;
  noGravityCounter = stateObj.noGravityCounter;
  gravityVal = stateObj.gravityVal;
  health = stateObj.health;
  velocityX = stateObj.velocityX;
  velocityY = stateObj.velocityY;
  frameLastAttackConnected = stateObj.frameLastAttackConnected;
  inCorner = stateObj.inCorner;
  active = stateObj.active;
  inHitStop = stateObj.inHitStop;
  gravity = stateObj.gravity;
  faceRight = stateObj.faceRight;
  inputFaceRight = stateObj.inputFaceRight;
  isDead = stateObj.isDead;
  updateFacing = stateObj.updateFacing;
  setCurrentState(stateObj.currentStateNum);
  currentState->loadState(stateObj.currentStateObj);
}

void Entity::setCurrentState(int stateDefNum){
  currentState = &stateList.at(stateDefNum-1);
}

std::pair<int,int> Entity::getPos(){
  return position;
};

StateDef* Entity::getCurrentState(){
  return currentState;
};

Mix_Chunk* Entity::getSoundWithId(int id){
  printf("getting sound item with ID:%d\n", id);
  return soundsEffects.at(id).sound;
};

int Entity::getSoundChannel(){
  return owner->getSoundChannel();
};

void Entity::setXPos(int x){
  position.first = x;
};

void Entity::setX(int x){
  position.first += x;
};

void Entity::setY(int y){
  position.second -= y;
};

void Entity::setFlag(ObjFlag flag){

  flags |= flag;
}

void Entity::clearFlag(ObjFlag flag){

  flags &= (~flag);
}

bool Entity::getFlag(ObjFlag flag){

  return (( flags & flag ) == flag);
}


void Entity::_changeState(int  stateNum){
  changeState(stateNum);
}

void Entity::_cancelState(int  stateNum){
  cancelState(stateNum);
}

void Entity::_velSetX(int ammount){
 velocityX = faceRight ? ammount : -ammount;
}

void Entity::_negVelSetX(int ammount){
 velocityX = faceRight ? -ammount : ammount;
}

void Entity::_velSetY(int ammount){
  velocityY = ammount;
}

void Entity::_moveForward(int ammount){
  faceRight ? setX(ammount) : setX(-ammount);
}

void Entity::_moveBack(int ammount){
  faceRight ? setX(-ammount) : setX(ammount);
}

void Entity::_moveUp(int ammount){
  setY(ammount);
}

void Entity::_moveDown(int ammount){
  setY(-ammount);
}

void Entity::_setControl(int val){
  control = val;
}

void Entity::_setCombo(int val){
  comboCounter = val;
}

void Entity::_setHitStun(int val){
  hitstun = val;
}

void Entity::_setGravity(int on){
  gravity = on;
}

void Entity::_setNoGravityCounter(int count){
  noGravityCounter = count;
}

void Entity::_resetAnim(){
  currentState->resetAnim();
}

void Entity::_activateEntity(int entityID){ 
}

void Entity::_setHitCancel(int val){
  currentState->canHitCancel = val;
}

void Entity::_setWhiffCancel(int val){
  currentState->canWhiffCancel = val;
}

void Entity::_deactivateEntity(int entityID){
  active = false;
}

void Entity::_snapToOpponent(int offset){
  auto opponentPos = owner->otherChar->getPos();
  bool opponentFaceRight = owner->otherChar->faceRight;

  position.first = opponentFaceRight ? (opponentPos.first + offset) : (opponentPos.first - offset);
  position.second = opponentPos.second;
}


int Entity::_getAnimTime(){
  return currentState->anim.timeRemaining();
}

int Entity::_getYPos(){
  int yPos = abs(getPos().second);
  return yPos;
}

int Entity::_getStateTime(){
  return currentState->stateTime;
}

int Entity::_getInput(int input){
  // printf("in entity getInput\n");
  Input inputType = VirtualController::inputMap[input](inputFaceRight);
  // printf("bruh\n");
  return virtualController->isPressed(inputType) ? 1 : 0;
}

int Entity::_getStateNum(){
  return currentState->stateNum;
}

int Entity::_getControl(){
  return control;
}

int Entity::_getAirActions(){
  return hasAirAction;
}

void Entity::_setCounter(int operand){
  currentState->counterHitFlag = operand;
}

void Entity::_setAirAction(int operand){
  hasAirAction = operand;
}

int Entity::_getCombo(){
  return comboCounter;
}

int Entity::_wasPressed(int input){
  return virtualController->wasPressedBuffer(VirtualController::inputMap[input](inputFaceRight)) ? 1 : 0;
}

int Entity::_getHitStun(){
  return hitstun;
}

int Entity::_getHitCancel(){
  return currentState->canHitCancel;
}

int Entity::_getWhiffCancel(){
  return currentState->canWhiffCancel;
}

int Entity::_getBlockStun(){
  return blockstun;

}

int Entity::_getMeter(){
  return 0;
}

int Entity::_getComebackMeter(){
  return 0;
}

void Entity::_addMeter(int i){
}

void Entity::_subtractMeter(int i){
}

int Entity::_getIsAlive(){
  return !isDead;
}

int Entity::_checkCommand(int commandIndex){
  return virtualController->checkCommand(commandIndex, inputFaceRight);
}

void Entity::_setBlockstun(int input){
  blockstun = input;
};

void Entity::_setInstall(int input){
};

int Entity::_getEntityStatus(int entityID){
  return 0;
};
int Entity::_getInstall(){
  return 0;
};

int Entity::getAnimScale(){
  return animScale;
};

int Entity::_getVelY(){
  return velocityY;
};

int Entity::_getVelX(){
  return velocityX;
};
