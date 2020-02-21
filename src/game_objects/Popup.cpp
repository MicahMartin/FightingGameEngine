#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include "game_objects/Popup.h"

Popup::Popup(){}
Popup::~Popup(){}

void Popup::update(){
  stateTime++;
  if (stateTime == playLength) {
    isActive = false;
  }
}

void Popup::draw(){
  anim.render(xPos, yPos, true, stateTime);
}

void Popup::loadDataFile(const char* filePath){
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

void Popup::incrementStateTime(){
  stateTime++;
}

void Popup::setStateTime(int newStateTime){
  stateTime = newStateTime;
}

int Popup::getStateTime(){
  return stateTime;
}
