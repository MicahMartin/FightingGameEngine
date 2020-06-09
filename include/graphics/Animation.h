#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"


struct AnimationElement {
  AnimationElement(int elemTime, int offsetX, int offsetY): elemTime(elemTime), offsetX(offsetX), offsetY(offsetY){};
  GameTexture gameTexture;
  int elemTime;
  bool infiniteFrame = false;
  int endTime;
  int offsetX;
  int offsetY;
  bool isYellow = false;
};

struct AnimationFrame {
  AnimationFrame(AnimationElement* animElem):animElem(animElem){};
  AnimationElement* animElem;
};

class Animation {
public:
  Animation();
  ~Animation();

  void loadAnimEvents(nlohmann::json json);
  // void render(int x, int y, bool faceRight, bool screenFreeze);
  // void render(int x, int y, bool faceRight, int stateTime, bool screenFreeze);
  void render(int x, int y, bool faceRight, int stateTime);
  void renderHitspark(int x, int y, bool faceRight);

  void setAnimTime(int time);
  void shake(int duration);
  void setAnimElemIndex(int index);
  void resetAnimEvents();
  int timeRemaining();
  void drawRect(SDL_Rect rect);

  std::pair<int, int> getDimensions(const char* path);

  int center = 0;
  int currentAnimElemIndex = 0;
  int currentAnimElemTimePassed = 0;
  int animationTime = 0;
  int animationTimePassed = 0;
  int animFrame = 0;

  bool hitShake = false;
  bool isRed = false;
  bool isGreen = false;
  bool isLight = false;
  int hitShakeToggler = 0;
private:
  /* data */
  std::vector<AnimationElement> animationElements;
  std::vector<int> frames;
  Graphics* graphics = Graphics::getInstance();
};

#endif
