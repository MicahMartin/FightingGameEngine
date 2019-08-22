#include "graphics/Animation.h"


Animation::Animation(){ }

Animation::~Animation(){ }

void Animation::loadAnimEvents(nlohmann::json json){
  animationTime = 0;
  animationTimePassed = 0;
  currentAnimElemIndex = 0;
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
  std::cout << "The current elem timePassed " << elem->elemTimePassed << std::endl;
  std::cout << "The current elem time" << elem->elemTime << std::endl;

  GameTexture* currentText = elem->gameTexture;
  currentText->setCords(x,y);
  currentText->render(faceRight);

  elem->elemTimePassed++;
  animationTimePassed++;
  if(elem->elemTimePassed == elem->elemTime){
    elem->elemTimePassed = 0;
    currentAnimElemIndex++;
  }
}

void Animation::setAnimTime(int time){
  animationTimePassed = time;
}

void Animation::setAnimElemIndex(int index){
  currentAnimElemIndex = index;
}

int Animation::timeRemaining(){
  std::cout << "The anim timePassed " << animationTimePassed << std::endl;
  std::cout << "The animTimeTotal" << animationTime << std::endl;
  return animationTime - animationTimePassed;
}
