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
};

StateDef::StateDef(nlohmann::json::value_type json, VirtualMachine* charVm) : charVm(charVm) {
  stateNum = json.at("state_num");
  loadFlags(json.at("flags"));

  std::string updateScriptTag = "updateScript:" + std::to_string(stateNum);
  std::string updateScriptError = "updateScript:" + std::to_string(stateNum) + "failed to compile";
  const char* updateScriptPath = json.at("update_script").get<std::string>().c_str();

  if(!charVm->compiler.compile(updateScriptPath, &updateScript, updateScriptTag.c_str())){
    updateScript.disassembleScript(updateScriptTag.c_str());
    throw std::runtime_error(updateScriptError);
  }

  // compile state's cancel script
  if(json.count("cancel_script")){
    std::string cancelScriptTag = "cancelScript:" + std::to_string(stateNum);
    std::string cancelScriptError = "cancelScript:" + std::to_string(stateNum) + "failed to compile";
    const char* cancelScriptPath = json.at("cancel_script").get<std::string>().c_str();

    if(!charVm->compiler.compile(cancelScriptPath, &cancelScript, cancelScriptTag.c_str())){
      throw std::runtime_error(cancelScriptError);
    }
  }

  if(json.count("sounds")){
    for (auto i : json.at("sounds").items()) {
      int soundID = i.value().at("soundID");
      int start = i.value().at("start");
      soundIndexMap[start].push_back(soundID);
    }
  }

  if(json.count("techState")){
    techState = json.at("techState");
  }

  // printf("done compiling\n");
  // printf("loading anim\n");
  loadAnimation(json.at("animation"));
  // printf("loading collisions\n");
  loadCollisionBoxes(json.at("collision_boxes"));
}

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
}

void StateDef::enter(){
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
  anim.render(position.first, position.second, faceRight, animTime);

  // TODO: Sound method
  if (soundIndexMap[stateTime].size() > 0) {
    for (int soundID : soundIndexMap[stateTime]) {
      // printf("trying to play soundID %d\n", soundID);
      Mix_PlayChannel(-1, owner->getSoundWithId(soundID), 0);
    }
  }

  //for(auto cb : pushBoxes) {
  //   if(cb->end == -1 || ( cb->start >= stateTime && cb->end < stateTime )){
  //     cb->render();
  //   }
  //}
  //for(auto cb : hurtBoxes) {
  //   if(cb->end == -1 || ( stateTime >= cb->start && stateTime < cb->end )){
  //     cb->render();
  //   }
  //}
  //for(auto cb : hitBoxes) {
  //   if(cb->end == -1 || ( stateTime >= cb->start && stateTime <= cb->end )){
  //     cb->render();
  //   }
  //}
  //for (auto cb : throwHitBoxes) {
  //  if(cb->end == -1 || ( stateTime >= cb->start && stateTime <= cb->end )){
  //    cb->render();
  //  }
  //}
};

void StateDef::loadFlags(nlohmann::json::value_type json){
  for(auto i : json.items()){
    std::string flag(i.value());
    flagByte |= flagMap[flag];
  }
};

void StateDef::loadAnimation(nlohmann::json json){
  anim.loadAnimEvents(json);
};

void StateDef::loadCollisionBoxes(nlohmann::json json){
  for(auto i : json.items()){
    // TODO: POLYMORPH THIS SHIT
    CollisionBox::CollisionType type = CollisionBox::collisionTypeMap.at(i.value().at("type"));
    int width = i.value().at("width");
    int height = i.value().at("height");
    int offsetX = i.value().at("offsetX");
    int offsetY = i.value().at("offsetY");
    int start = i.value().at("start");
    int end = i.value().at("end");

    CollisionBox* cb;
    if(type == CollisionBox::HIT || type == CollisionBox::THROW){
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
    }
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
