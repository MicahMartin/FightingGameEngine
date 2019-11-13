#include <iostream>
#include <functional>
#include "character_state/StateDef.h"
#include "game_objects/Character.h"
#include "physics/CollisionBox.h"
#include "Util.h"

StateDef::StateDef(nlohmann::json::value_type json, Character* player, VirtualMachine* charVm) : player(player), charVm(charVm) {
    stateNum = json.at("state_num");
    loadFlags(json.at("flags"));

    std::string updateScriptTag = "updateScript:" + std::to_string(stateNum);
    std::string updateScriptError = "updateScript:" + std::to_string(stateNum) + "failed to compile";
    const char*  updateScriptPath = json.at("update_script").get<std::string>().c_str();

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
      // delete cancelSource;
    }
    printf("done compiling\n");
    printf("loading anim\n");
    loadAnimation(json.at("animation"));
    printf("loading collisions\n");
    loadCollisionBoxes(json.at("collision_boxes"));
}

StateDef::~StateDef(){ }

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

    std::pair charPos = player->getPos();

    CollisionBox* cb;
    if(i.value().contains("hitscript")){
      // TODO: Fix collisionbox loading
      cb = new CollisionBox(type, width, height, offsetX, offsetY, start, end, 
          i.value().at("hitscript"), i.value().at("damage"), i.value().at("push"), i.value().at("hitstop"), i.value().at("hitstun"));
      if (i.value().contains("canTrip")) {
        cb->canTrip = true;
      }

    } else {
      cb = new CollisionBox(type, width, height, offsetX, offsetY, start, end);
    }

    cb->positionX = charPos.first - (width/2);
    cb->positionY = charPos.second;

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
    }
  }
};

void StateDef::enter(){
  stateTime = 0;
  anim.resetAnimEvents();
  // TODO: move updateCollisionBoxes into here, shouldnt belong to player
  player->updateCollisionBoxes();
};

void StateDef::update(){
  charVm->execute(&updateScript);
  stateTime++;
}

void StateDef::handleCancels(){
  if(cancelScript.code.size()){
    charVm->execute(&cancelScript);
  }
}

void StateDef::draw(){
  std::pair charPos = player->getPos();
  bool faceRight = player->faceRight;
  anim.render(charPos.first, charPos.second, faceRight, charStateManager->screenFrozen);

  // stateTime is 2
  // TODO: Thread this up
  for (auto cb : pushBoxes) {
    if(cb->end == -1 || ( cb->start >= stateTime && cb->end < stateTime )){
      cb->render();
    }
  }
  for (auto cb : hurtBoxes) {
    if(cb->end == -1 || ( stateTime >= cb->start && stateTime < cb->end )){
      cb->render();
    }
  }
  for (auto cb : hitBoxes) {
    if(cb->end == -1 || ( stateTime >= cb->start && stateTime < cb->end )){
      cb->render();
    }
  }
};

void StateDef::resetAnim(){
  anim.setAnimTime(0);
  anim.resetAnimEvents();
}

bool StateDef::checkFlag(FlagBit bit){
  return flagByte & bit;
};
