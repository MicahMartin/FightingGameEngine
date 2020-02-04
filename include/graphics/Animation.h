#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include "graphics/GameTexture.h"
#include "graphics/Graphics.h"

class Animation {
public:
  Animation();
  ~Animation();

  void loadAnimEvents(nlohmann::json json);
  void render(int x, int y, bool faceRight, bool screenFreeze);
  void render(int x, int y, bool faceRight, int stateTime, bool screenFreeze);
  void renderHitspark(int x, int y, bool faceRight);

  void setAnimTime(int time);
  void shake(int duration);
  void setAnimElemIndex(int index);
  void resetAnimEvents();
  int timeRemaining();
  void drawRect(SDL_Rect rect);

  std::pair<int, int> getDimensions(const char* path);

  struct AnimationElement {
    AnimationElement(int elemTime, int offsetX, int offsetY): elemTime(elemTime), offsetX(offsetX), offsetY(offsetY){ }
    GameTexture gameTexture;
    int elemTime;
    int endTime;
    int offsetX;
    int offsetY;
  };

  int center;
  int currentAnimElemIndex;
  int currentAnimElemTimePassed;
  int animationTime;
  int animationTimePassed;
private:
  /* data */
  std::vector<AnimationElement> animationElements;
  Graphics* graphics = Graphics::getInstance();
};

#endif
