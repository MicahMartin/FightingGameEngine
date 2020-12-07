#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "game_objects/Popup.h"

Popup::Popup(){}
Popup::~Popup(){}

void Popup::update(){
  if (stateTime == playLength) {
    isActive = false;
  }
  stateTime++;
}

void Popup::draw(){
  anim.render(xPos/10, yPos/10, true, stateTime);
}

void Popup::reset(){
  isActive = false;
  stateTime = 0;
  xPos = 0;
  yPos = 0;
  playLength = 0;
}

void Popup::loadDataFile(const char* filePath){
  reset();
  std::ifstream dataFile(filePath);
  nlohmann::json dataJson;
  dataFile >> dataJson;

  anim.loadAnimEvents(dataJson.at("animation"));
  dataFile.close();
}

std::pair<int,int> Popup::getPos(){
  return std::pair<int, int>(xPos, yPos);
}

void Popup::setX(int x){
  xPos = x;
}
void Popup::setY(int y){
  yPos = y;
}
int Popup::getX(){
  return xPos;
}
int Popup::getY(){
  return yPos;
}

void Popup::setActive(bool active){
  isActive = active;
}

bool Popup::getActive(){
  return isActive;
}

void Popup::setPlayLength(int newPlayLength){
  playLength = newPlayLength;
}

int Popup::getPlayLength(){
  return playLength;
}

void Popup::incrementStateTime(){
  stateTime++;
}

void Popup::setStateTime(int newStateTime){
  stateTime = newStateTime;
}

int Popup::getStateTime(){
  return stateTime;
}
