#ifndef _ANIMATION_H
#define _ANIMATION_H

#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include "graphics/GameTexture.h"

class Animation {
public:
  Animation();
  ~Animation();

  void loadAnimEvents(nlohmann::json json);
  void render(int x, int y, bool faceRight);

  void setAnimTime(int time);
  void setAnimElemIndex(int index);
  void resetAnimEvents();
  int timeRemaining();

  std::pair<int, int> getDimensions(const char* path);

  struct AnimationElement {
    AnimationElement(GameTexture* gameTexture, int elemTime): gameTexture(gameTexture), elemTime(elemTime){
      elemTime = 0;
    }
    GameTexture* gameTexture;
    int elemTime;
  };

private:
  /* data */
  std::vector<AnimationElement> animationElements;
  int currentAnimElemIndex;
  int currentAnimElemTimePassed;
  int animationTime;
  int animationTimePassed;
};

#endif
