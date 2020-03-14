#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "game_objects/VisualEffect.h"

VisualEffect::VisualEffect(){}
VisualEffect::~VisualEffect(){}

void VisualEffect::update(){
  if (stateTime == playLength) {
    isActive = false;
    stateTime = 0;
  } else if (isActive) {
    stateTime++;
  }
}

void VisualEffect::draw(){
  anim.render(xPos, yPos, true, stateTime);
}

void VisualEffect::draw(bool faceRight){
  anim.render(xPos, yPos, faceRight, stateTime);
}

void VisualEffect::reset(int _xPos, int _yPos){
  isActive = false;
  stateTime = 0;
  xPos = _xPos;
  yPos = _yPos;
}

std::pair<int,int> VisualEffect::getPos(){
  return std::pair<int, int>(xPos, yPos);
}

void VisualEffect::setX(int x){
  xPos = x;
}
void VisualEffect::setY(int y){
  yPos = y;
}

int VisualEffect::getX(){
  return xPos;
}
int VisualEffect::getY(){
  return yPos;
}

void VisualEffect::setActive(bool active){
  isActive = active;
}

bool VisualEffect::getActive(){
  return isActive;
}

void VisualEffect::incrementStateTime(){
  stateTime++;
}

void VisualEffect::setStateTime(int newStateTime){
  stateTime = newStateTime;
}

int VisualEffect::getStateTime(){
  return stateTime;
}

void VisualEffect::setPlayLength(int newPlayLength){
  playLength = newPlayLength;
}

int VisualEffect::getPlayLength(){
  return playLength;
}
