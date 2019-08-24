#include "graphics/Animation.h"


Animation::Animation(){ }

Animation::~Animation(){ }

void Animation::loadAnimEvents(nlohmann::json json){
  currentAnimElemIndex = 0;
  currentAnimElemTimePassed = 0;
  animationTime = 0;
  animationTimePassed = 0;
  for (auto i : json.items()) {
    GameTexture* text = new GameTexture();
    text->cartesian = true;
    text->loadTexture(i.value().at("file").get<std::string>().c_str());
    text->setDimensions(0, 0, 150, 200);

    int animTime = i.value().at("time");
    animationTime += animTime;

    AnimationElement element(text, animTime);
    animationElements.push_back(element);
  }
}

void Animation::render(int x, int y, bool faceRight){
  AnimationElement* elem = &animationElements.at(currentAnimElemIndex);

  GameTexture* currentText = elem->gameTexture;
  currentText->setCords(x,y);
  currentText->render(faceRight);

  currentAnimElemTimePassed++;
  animationTimePassed++;
  if(currentAnimElemTimePassed == elem->elemTime){
    currentAnimElemTimePassed = 0;
    currentAnimElemIndex++;
  }
}

void Animation::setAnimTime(int time){
  animationTimePassed = time;
}

void Animation::setAnimElemIndex(int index){
  currentAnimElemIndex = index;
}

void Animation::resetAnimEvents(){
  currentAnimElemIndex = 0;
  currentAnimElemTimePassed = 0;
}

int Animation::timeRemaining(){
  return animationTime - animationTimePassed;
}
