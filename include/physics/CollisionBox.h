#ifndef _CollisionBox_h
#define _CollisionBox_h

#include "graphics/Graphics.h"
#include <utility>
#include <string>
#include <map>


class CollisionBox {
public:

  enum CollisionType {
    POSITION,
    HURT
  };


  static bool checkAABB(CollisionBox box1, CollisionBox box2);
  static const std::map<std::string, CollisionType> collisionTypeMap;

  CollisionBox(CollisionType boxType, int width, int height, int offsetX, int offsetY);
  ~CollisionBox();

  void render();
  

  int positionX;
  int positionY;
  int width;
  int height; 

  int offsetX; 
  int offsetY;
private:
  Graphics* graphics = Graphics::getInstance();
  CollisionType boxType;
};

#endif
