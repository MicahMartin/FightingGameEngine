#include "physics/CollisionBox.h"
#include "graphics/Camera.h"
// TODO: STOP BEING LAZY AND POLY THIS

CollisionBox::CollisionBox(CollisionBox::CollisionType boxType, int width, int height, 
  int offsetX, int offsetY, int start, int end) 
: CollisionBox(boxType, width, height, offsetX, offsetY, start, end, 0,0,0,0,0,0,0) {}

CollisionBox::CollisionBox(CollisionBox::CollisionType boxType, int width, int height, 
  int offsetX, int offsetY, int start, int end, int damage, int pushback, int hitstop, int hitstun, int pushTime, int blockstun, int blockType)
: boxType(boxType), width(width), height(height), offsetX(offsetX), offsetY(offsetY), start(start), 
  end(end),damage(damage), pushback(pushback), hitstop(hitstop), hitstun(hitstun), pushTime(pushTime), blockstun(blockstun), blockType(blockType){ }

CollisionBox::~CollisionBox(){ }

bool CollisionBox::checkAABB(CollisionBox box1, CollisionBox box2){
  std::pair<int, int> b1Pos(box1.positionX, box1.positionY + (720-box1.height));
  std::pair<int, int> b2Pos(box2.positionX, box2.positionY + (720-box2.height));
  // printf("checking {t: %d, x: %d, y:%d, w:%d, l:%d} against {t: %d, x: %d, y:%d, w:%d, l:%d}\n", box1.boxType,
  //     box1.positionX, box1.positionY, box1.width, box1.height, box2.boxType, box2.positionX, box2.positionY, box2.width, box2.height);

  // {t: 2, x: 707, y:-125, w:60, l:60} against {t: 1, x: 730, y:0, w:100, l:200}
  if(box1.positionX < box2.positionX + box2.width &&
     box1.positionX + box1.width > box2.positionX &&
     box1.positionY > box2.positionY - box2.height &&
     box1.positionY - box1.height < box2.positionY) {

     return true;
  }


  return false;
};


void CollisionBox::render(){
  Camera* cam = graphics->getCamera();
  switch (boxType) {
    case POSITION:
      SDL_SetRenderDrawColor(graphics->getRenderer(), 0, 0, 255, 0);
      break;
    case HURT:
      SDL_SetRenderDrawColor(graphics->getRenderer(), 0, 255, 0, 0);
      break;
    case HIT:
      SDL_SetRenderDrawColor(graphics->getRenderer(), 255, 0, 0, 0);
      break;
    case THROW:
      SDL_SetRenderDrawColor(graphics->getRenderer(), 255, 0, 255, 0);
      break;
  }
  SDL_Rect collisionRect;

  collisionRect.x = (positionX - cam->cameraRect.x);
  collisionRect.y = (positionY + (graphics->getWindowHeight() - height) - 30) + cam->cameraRect.y;
  collisionRect.w = width;
  collisionRect.h = height;

  SDL_RenderDrawRect(graphics->getRenderer(), &collisionRect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}


std::map<std::string, CollisionBox::CollisionType> CollisionBox::collisionTypeMap = {
  {"POSITION", CollisionBox::POSITION},
  {"HURT", CollisionBox::HURT},
  {"HIT", CollisionBox::HIT},
  {"THROW", CollisionBox::THROW},
};
