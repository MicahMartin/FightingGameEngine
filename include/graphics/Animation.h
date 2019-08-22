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
  int timeRemaining();

  struct AnimationElement {
    AnimationElement(GameTexture* gameTexture, int elemTime): gameTexture(gameTexture), elemTime(elemTime){
      elemTimePassed = 0;
      elemTime = 0;
    }
    GameTexture* gameTexture;
    int elemTime;
    int elemTimePassed;
  };

private:
  /* data */
  std::vector<AnimationElement> animationElements;
  int currentAnimElemIndex;
  int animationTime;
  int animationTimePassed;
};

#endif
