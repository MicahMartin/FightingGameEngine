#include "graphics/Animation.h"
#include <fstream>


Animation::Animation(){ }

Animation::~Animation(){ }

void Animation::loadAnimEvents(nlohmann::json json){
  currentAnimElemIndex = 0;
  currentAnimElemTimePassed = 0;
  animationTime = 0;
  animationTimePassed = 0;
  for (auto i : json.items()) {
    GameTexture* text = new GameTexture();
    const char* path = i.value().at("file").get<std::string>().c_str();
    std::pair dimensions = getDimensions(path);

    text->cartesian = true;
    text->loadTexture(path);
    text->setDimensions(0, 0, dimensions.first*2, dimensions.second*2);

    int animTime = i.value().at("time");
    int offsetX = i.value().at("offsetX");
    animationTime += animTime;

    AnimationElement element(text, animTime, offsetX);
    animationElements.push_back(element);
  }
}

void Animation::render(int x, int y, bool faceRight, bool screenFreeze){
  // draw a vertical line on the character's position
  SDL_SetRenderDrawColor(graphics->getRenderer(), 255, 0, 0, 0);
  SDL_RenderDrawLine(graphics->getRenderer(), x, graphics->getWindowHeight(), x, y);
  SDL_SetRenderDrawColor(graphics->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

  AnimationElement* elem = &animationElements.at(currentAnimElemIndex);

  GameTexture* currentText = elem->gameTexture;
  int width = currentText->getDimensions().first;
  int offsetX = elem->offsetX;
  faceRight ? currentText->setCords(x-width+offsetX, y - 30) : currentText->setCords(x-offsetX, y - 30);
  currentText->render(faceRight);

  if(!screenFreeze){
    currentAnimElemTimePassed++;
    animationTimePassed++;
    if(currentAnimElemTimePassed == elem->elemTime){
      currentAnimElemTimePassed = 0;
      currentAnimElemIndex++;
    }
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

std::pair<int, int> Animation::getDimensions(const char* path){
    std::pair<int, int> returnPair;
    unsigned int width, height;

    std::ifstream img(path);

    // width and height are offset by 16 bytes
    // ty TCP training, everything has a TLV
    img.seekg(16);
    img.read((char *)&width, 4);
    img.read((char *)&height, 4);

    returnPair.first = ntohl(width);
    returnPair.second = ntohl(height);

    return returnPair;
}
