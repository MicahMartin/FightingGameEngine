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
    HURT,
    HIT
  };


  static bool checkAABB(CollisionBox box1, CollisionBox box2);
  static std::map<std::string, CollisionType> collisionTypeMap;

  CollisionBox(CollisionType boxType, int width, int height, int offsetX, int offsetY, int start, int end);
  CollisionBox(CollisionType boxType, 
      int width, int height, int offsetX, int offsetY, int start, int end, int hitScript, int damage, int pushback, int hitstop, int hitstun);
  ~CollisionBox();

  void render();
  

  int positionX;
  int positionY;
  int width;
  int height; 

  int offsetX; 
  int offsetY;

  int start;
  int end;

  bool disabled = false; 

  // i know, poly
  int hitScript;
  int damage;
  int pushback;
  int hitstop;
  int hitstun;

private:
  Graphics* graphics = Graphics::getInstance();
  CollisionType boxType;
};

#endif
