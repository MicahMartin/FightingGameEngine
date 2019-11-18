#include "graphics/Animation.h"
#include <fstream>


Animation::Animation(){ 
  currentAnimElemIndex = 0;
  currentAnimElemTimePassed = 0;
  animationTime = 0;
  animationTimePassed = 0;
}

Animation::~Animation(){ }

void Animation::loadAnimEvents(nlohmann::json json){
  for (auto i : json.items()) {
    int animTime = i.value().at("time");
    int offsetX = 0;
    int offsetY = 0;
    if(i.value().count("offsetY")){
      offsetY = i.value().at("offsetY");
    }
    if(i.value().count("offsetX")){
      offsetX = i.value().at("offsetX");
    }
    AnimationElement element(animTime, offsetX, offsetY);

    GameTexture* text = &element.gameTexture;
    text->cartesian = true;
    const char* path = i.value().at("file").get<std::string>().c_str();
    std::pair dimensions = getDimensions(path);
    text->loadTexture(path);
    text->setDimensions(0, 0, dimensions.first*3, dimensions.second*3);

    animationTime += animTime;
    animationElements.push_back(element);
  }
}

void Animation::render(int x, int y, bool faceRight, bool screenFreeze){
  // draw a vertical line on the character's position
  int camOffset = graphics->getCamera()->cameraRect.x;
  // SDL_SetRenderDrawColor(graphics->getRenderer(), 255, 0, 0, 0);
  // SDL_RenderDrawLine(graphics->getRenderer(), x - camOffset, graphics->getWindowHeight(), x - camOffset, y);
  // SDL_SetRenderDrawColor(graphics->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);

  AnimationElement* elem = &animationElements.at(currentAnimElemIndex);

  GameTexture* currentText = &elem->gameTexture;
  int width = currentText->getDimensions().first;
  int offsetX = elem->offsetX;
  int offsetY = elem->offsetY;
  faceRight ? currentText->setCords(x-width+offsetX, ((y - 30) + offsetY)) : currentText->setCords(x-offsetX, ((y - 30) + offsetY));
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
  animationTimePassed = 0;
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
    img.close();

    return returnPair;
}
