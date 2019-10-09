#include <iostream>
#include <functional>
#include "character_state/StateDef.h"
#include "game_objects/Character.h"
#include "physics/CollisionBox.h"
#include "Util.h"

StateDef::StateDef(int charNum, int stateNum) : charNum(charNum), stateNum(stateNum){ }
StateDef::~StateDef(){ }

void StateDef::loadFlags(nlohmann::json json){
  for(auto i : json.items()){
    std::string flag(i.value());
    flagByte |= flagMap[flag];
  }
};

void StateDef::loadAnimation(nlohmann::json json){
  anim.loadAnimEvents(json);
};

void StateDef::loadByteCode(nlohmann::json json){
  byteCode = ByteCode::compile(json);
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

    Character* player = charStateManager->getCharPointer(charNum);
    std::pair charPos = player->getPos();

    CollisionBox* cb;
    if(i.value().contains("hitscript")){
      cb = new CollisionBox(type, width, height, offsetX, offsetY, start, end, i.value().at("hitscript"));

    } else {
      cb = new CollisionBox(type, width, height, offsetX, offsetY, start, end);
    }

    cb->positionX = charPos.first - (width/2);
    cb->positionY = charPos.second;

    switch (type) {
      case CollisionBox::POSITION:
        pushBoxes.push_back(*cb);
        break;
      case CollisionBox::HURT:
        hurtBoxes.push_back(*cb);
        break;
      case CollisionBox::HIT:
        hitBoxes.push_back(*cb);
        break;
    }
  }
};

void StateDef::enter(){
  anim.setAnimTime(0);
  anim.resetAnimEvents();
  for (auto &i : hitBoxes) {
    i.disabled = true;
  }
  stateTime = 0;
};

void StateDef::update(){
  VirtualMachine* charVM = &charStateManager->getCharPointer(charNum)->virtualMachine;
  charVM->execute(&byteCode[0], byteCode.size(), 0);
  stateTime++;
}

void StateDef::draw(){
  std::pair charPos = charStateManager->getCharPointer(charNum)->getPos();
  bool faceRight = charStateManager->getCharPointer(charNum)->faceRight;
  anim.render(charPos.first, charPos.second, faceRight, charStateManager->screenFrozen);

  // stateTime is 2
  // TODO: Thread this up
  for (auto cb : pushBoxes) {
    if(cb.end == -1 || ( cb.start >= stateTime && cb.end <= stateTime )){
      cb.render();
    }
  }
  for (auto cb : hurtBoxes) {
    if(cb.end == -1 || ( stateTime >= cb.start && stateTime <= cb.end )){
      cb.render();
    }
  }
  for (auto cb : hitBoxes) {
    if(cb.end == -1 || ( stateTime >= cb.start && stateTime <= cb.end )){
      cb.render();
    }
  }
};

bool StateDef::checkFlag(FlagBit bit){
  return flagByte & bit;
};
