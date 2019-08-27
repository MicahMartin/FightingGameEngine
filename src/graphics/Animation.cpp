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
    text->setDimensions(0, 0, dimensions.first*1.5, dimensions.second*1.5);

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

    std::cout << "IMAGE: " << path << " DIMENSIONS: w[" << returnPair.first << "] h[" << returnPair.second << "]" << std::endl;
    return returnPair;
}
