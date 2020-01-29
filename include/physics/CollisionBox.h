#ifndef _CollisionBox_h
#define _CollisionBox_h

#include "graphics/Graphics.h"
#include <utility>
#include <SDL_mixer.h>
#include <string>
#include <map>


// TODO: poly this whn you find a break in fun stuff
struct CollisionRect {
  int x, y, w, h;
};
class CollisionBox {
public:

  enum CollisionType {
    POSITION,
    HURT,
    HIT,
    THROW 
  };


  static bool checkAABB(CollisionBox box1, CollisionBox box2);
  static CollisionRect getAABBIntersect(CollisionBox box1, CollisionBox box2);
  static std::map<std::string, CollisionType> collisionTypeMap;

  CollisionBox(CollisionType boxType, int width, int height, int offsetX, 
      int offsetY, int start, int end);

  CollisionBox(CollisionType boxType, int width, int height, int offsetX, 
      int offsetY, int start, int end, int damage, int pushback, int hitstop, int hitstun, int pushTime, int blockstun, int blocktype);

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
  int damage;
  int pushback;
  int pushTime;
  int hitstop;
  int hitstun;
  int blockstun;
  int blockType;
  int throwType;
  int success;
  int opponentState;
  int groupID = 1;
  int hitSoundID = 0;
  int guardSoundID = 0;
  bool canTrip = false;

private:
  Graphics* graphics = Graphics::getInstance();
  CollisionType boxType;
};

#endif
