#include <iostream>
#include <functional>
#include "character_state/StateDef.h"
#include "physics/CollisionBox.h"
#include "util/Util.h"
#include "game_objects/Character.h"

 std::map<std::string, FlagBit> StateDef::flagMap = {
  {"NO_TURN", NO_TURN},
  {"NO_TURN_ON_ENTER", NO_TURN_ON_ENTER},
  {"TECHABLE", TECHABLE},
  {"SUPER_ATTACK", SUPER_ATTACK},
};

StateDef::StateDef(){}

StateDef::~StateDef() {
  // TODO: DeleteCB method
  for (auto cb : pushBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
  for (auto cb : hurtBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
  for (auto cb : hitBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
  for (auto cb : throwHitBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
  for (auto cb : throwHurtBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
  for (auto cb : proximityBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
  for (auto cb : projectileBoxes) {
    if (cb != NULL) {
      delete cb;
    }
  }
}

void StateDef::init(nlohmann::json::value_type json, VirtualMachine* _charVm, float _animScale){
  charVm = _charVm;
  animScale = _animScale;

  stateNum = json.at("state_num");
  loadFlags(json.at("flags"));

  std::string updateScriptTag = "updateScript:" + std::to_string(stateNum);
  std::string updateScriptError = "updateScript:" + std::to_string(stateNum) + "failed to compile";
  std::string updateScriptStr = json.at("update_script").get<std::string>();
  const char* updateScriptPath = updateScriptStr.c_str();
  printf("the updatescript %s\n", updateScriptPath);

  if(!charVm->compiler.compile(updateScriptPath, &updateScript, updateScriptTag.c_str())){
    updateScript.disassembleScript(updateScriptTag.c_str());
    throw std::runtime_error(updateScriptError);
  }

  // compile state's cancel script
  if(json.count("cancel_script")){
    std::string cancelScriptTag = "cancelScript:" + std::to_string(stateNum);
    std::string cancelScriptError = "cancelScript:" + std::to_string(stateNum) + "failed to compile";
    std::string cancelScriptStr = json.at("cancel_script").get<std::string>();
    const char* cancelScriptPath = cancelScriptStr.c_str();

    if(!charVm->compiler.compile(cancelScriptPath, &cancelScript, cancelScriptTag.c_str())){
      throw std::runtime_error(cancelScriptError);
    }
  }

  if(json.count("freeze_frame") && json.count("freeze_length")){
    freezeFrame = json.at("freeze_frame");
    freezeLength = json.at("freeze_length");
  }

  if(json.count("techState")){
    techState = json.at("techState");
  }

  if(json.count("sounds")){
    for (auto i : json.at("sounds").items()) {
      int soundID = i.value().at("soundID");
      int start = i.value().at("start");
      soundIndexMap[start].push_back(soundID);
    }
  }

  // printf("done compiling\n");
  printf("loading anim\n");
  anim.charName = charName;
  loadAnimation(json.at("animation"));
  printf("loading collisions\n");
  loadCollisionBoxes(json.at("collision_boxes"));
  printf("loading visual effects\n");
  if (json.count("visual_effects")) {
    loadVisualEffects(json.at("visual_effects"));
  }
  printf("done loading stateDef\n");
}



StateDefObj StateDef::saveState(){
  StateDefObj stateObj;
  // std::unordered_map<CollisionBox*, CollisionBoxState> cbStates;
  // for (auto i : collisionBoxes) {
  //   cbStates[i] = i->saveState();
  // }

  stateObj.stateTime = stateTime;
  stateObj.animTime = animTime;
  stateObj.freezeFrame = freezeFrame;
  stateObj.freezeLength = freezeLength;
  stateObj.hitboxesDisabled = hitboxesDisabled;
  stateObj.canWhiffCancel = canWhiffCancel;
  stateObj.canHitCancel = canHitCancel;
  stateObj.counterHitFlag = counterHitFlag;
  for (int i = 0; i < hitboxGroupDisabled.size(); ++i) {
    stateObj.hitboxGroupDisabled[i] = hitboxGroupDisabled[i];
  }
  for (int i = 0; i < collisionBoxes.size(); ++i) {
    stateObj.collisionBoxStates[i] = collisionBoxes[i]->saveState();
  }
  // stateObj.collisionBoxStates = cbStates;
 
  return stateObj;
}

void StateDef::loadState(StateDefObj stateObj){
  stateTime = stateObj.stateTime;
  animTime = stateObj.animTime;
  freezeFrame = stateObj.freezeFrame;
  freezeLength = stateObj.freezeLength;
  hitboxesDisabled = stateObj.hitboxesDisabled;
  canWhiffCancel = stateObj.canWhiffCancel;
  canHitCancel = stateObj.canHitCancel;
  counterHitFlag = stateObj.counterHitFlag;
  for (int i = 0; i < hitboxGroupDisabled.size(); ++i) {
    hitboxGroupDisabled[i] = stateObj.hitboxGroupDisabled[i];
  }
  for (int i = 0; i < collisionBoxes.size(); ++i) {
    collisionBoxes[i]->loadState(stateObj.collisionBoxStates[i]);
  }
}

void StateDef::enter(){
  counterHitFlag = false;
  canHitCancel = false;
  canWhiffCancel = false;
  stateTime = 0;
  animTime = 0;
  hitboxesDisabled = false;
  for (auto& it: hitboxGroupDisabled) {
    it.second = false;
  }
  anim.resetAnimEvents();
};

void StateDef::update(){
  charVm->execute(&updateScript);
  stateTime++;
  animTime++;
}

void StateDef::handleCancels(){
  if(cancelScript.code.size()){
    charVm->execute(&cancelScript);
  }
}

void StateDef::draw(std::pair<int,int> position, bool faceRight, bool inHitStop){
  anim.hitShake = inHitStop;
  anim.render(position.first/COORDINATE_SCALE, position.second/COORDINATE_SCALE, faceRight, animTime);

  if (Graphics::getInstance()->getFlag(GF_SHOW_CB)) {
    drawCollisionBoxes();
  }
};

void StateDef::drawCollisionBoxes(){
  for(auto cb : pushBoxes) {
     if(!cb->disabled){
       cb->render();
     }
  }
  for(auto cb : hitBoxes) {
     if(!cb->disabled){
       cb->render();
     }
  }
  for (auto cb : throwHitBoxes) {
    if(!cb->disabled){
      cb->render();
    }
  }
  for (auto cb : throwHurtBoxes) {
    if(!cb->disabled){
      cb->render();
    }
  }
  for (auto cb : proximityBoxes) {
    if(!cb->disabled){
      cb->render();
    }
  }
  for (auto cb : projectileBoxes) {
    if(!cb->disabled){
      cb->render();
    }
  }
  for(auto cb : hurtBoxes) {
     if(!cb->disabled){
       cb->render();
     }
  }
}

void StateDef::loadFlags(nlohmann::json::value_type json){
  for(auto i : json.items()){
    std::string flag(i.value());
    flagByte |= flagMap[flag];
  }
};

void StateDef::loadAnimation(nlohmann::json json){
  anim.loadAnimEvents(animScale, json);
};

void StateDef::loadCollisionBoxes(nlohmann::json json){
  for(auto i : json.items()){
    // TODO: POLYMORPH THIS SHIT
    CollisionBox::CollisionType type = CollisionBox::collisionTypeMap.at(i.value().at("type"));
    int scale = COORDINATE_SCALE;
    int width = i.value()["width"].get<int>() * scale;
    int height = i.value()["height"].get<int>() * scale;
    int offsetX = i.value()["offsetX"].get<int>() * scale;
    int offsetY = i.value()["offsetY"].get<int>() * scale;
    int start = i.value().at("start");
    int end = i.value().at("end");

    CollisionBox* cb;
    if(type == CollisionBox::HIT || type == CollisionBox::THROW || type == CollisionBox::PROJECTILE){
      // TODO: Fix collisionbox loading
      cb = new CollisionBox(type, width, height, offsetX, offsetY, start, end, 
          i.value().at("damage"), i.value().at("push"), i.value().at("hitstop"), 
          i.value().at("hitstun"), i.value().at("push_time"), i.value().at("block_stun"), 
          i.value().at("block_type"));
      if(type == CollisionBox::THROW){
        cb->throwType = i.value().at("throw_type");
        cb->throwSuccess = i.value().at("throwSuccess");
        cb->opponentThrowSuccess = i.value().at("opponentThrowSuccess");
        cb->throwAttempt = i.value().at("throwAttempt");
        cb->techAttempt = i.value().at("techAttempt");
      }
      if(type == CollisionBox::HIT || type == CollisionBox::PROJECTILE){
        cb->guardsparkID = 1;
        cb->hitsparkID = 1;
        if (i.value().count("meter")) {
          cb->hitMeterGain = i.value().at("meter");
        }
        if (i.value().count("guardsparkID")) {
          cb->guardsparkID = i.value().at("guardsparkID");
        }
        if (i.value().count("hitsparkID")) {
          cb->hitsparkID = i.value().at("hitsparkID");
        }
      }
      if (i.value().count("group")) {
        cb->groupID = i.value().at("group");
        hitboxGroupDisabled[cb->groupID] = false;
      }
      if (i.value().contains("canTrip")) {
        cb->canTrip = true;
      }
      if (i.value().count("hitsound")) {
        cb->hitSoundID = i.value().at("hitsound");
      }
      if (i.value().count("guardsound")) {
        cb->guardSoundID = i.value().at("guardsound");
      }
      if (i.value().count("hit_type")) {
        cb->hitType = i.value().at("hit_type");
      }
      if (i.value().count("hit_velX")) {
        cb->hitVelocityX = i.value().at("hit_velX");
      }
      if (i.value().count("hit_velY")) {
        cb->hitVelocityY = i.value().at("hit_velY");
      }
      if (i.value().count("hit_push_time")) {
        cb->hitPushTime = i.value().at("hit_push_time");
      }
      if (i.value().count("air_hitstun")) {
        cb->airHitstun = i.value().at("air_hitstun");
      }
      if (i.value().count("air_hitvelX")) {
        cb->airHitVelocityX = i.value().at("air_hitvelX");
      }
      if (i.value().count("air_hitpushtime")) {
        cb->airHitPushTime = i.value().at("air_hitpushtime");
      }

    } else {
      cb = new CollisionBox(type, width, height, offsetX, offsetY, start, end);
    }

    cb->positionX = 0;
    cb->positionY = 0;

    switch (type) {
      case CollisionBox::POSITION:
        pushBoxes.push_back(cb);
        break;
      case CollisionBox::HURT:
        hurtBoxes.push_back(cb);
        break;
      case CollisionBox::HIT:
        hitBoxes.push_back(cb);
        break;
      case CollisionBox::THROW:
        throwHitBoxes.push_back(cb);
        break;
      case CollisionBox::THROW_HURT:
        throwHurtBoxes.push_back(cb);
        break;
      case CollisionBox::PROXIMITY:
        proximityBoxes.push_back(cb);
        break;
      case CollisionBox::PROJECTILE:
        projectileBoxes.push_back(cb);
        break;
    }
    collisionBoxes.push_back(cb);
  }
};

void StateDef::loadVisualEffects(nlohmann::json::value_type json){
  for(auto i : json.items()){
    int startFrame = i.value().at("start");
    int visualID = i.value().at("visualID");
    visualEffectMap.emplace(startFrame, visualID);
  }
};

void StateDef::resetAnim(){
  animTime = 0;
  anim.setAnimTime(0);
  anim.resetAnimEvents();
}

bool StateDef::checkFlag(FlagBit bit){
  return flagByte & bit;
};
