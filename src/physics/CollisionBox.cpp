#include "physics/CollisionBox.h"

CollisionBox::CollisionBox(CollisionBox::CollisionType boxType, int width, int height, int offsetX, int offsetY) 
  : boxType(boxType), width(width), height(height), offsetX(offsetX), offsetY(offsetY) { }

CollisionBox::~CollisionBox(){ }

bool CollisionBox::checkAABB(CollisionBox box1, CollisionBox box2){
  std::pair<int, int> b1Pos(box1.positionX, box1.positionY + (720-box1.height));
  std::pair<int, int> b2Pos(box2.positionX, box2.positionY + (720-box2.height));

  if(box1.positionX < box2.positionX + box2.width &&
     box1.positionX + box1.width > box2.positionX &&
     box1.positionY < box2.positionY + box2.height &&
     box1.positionY + box1.height > box2.positionY) 
  {
     printf("collision detected\n");
     return true;
  }


  return false;
};


void CollisionBox::render(){
  SDL_SetRenderDrawColor(graphics->getRenderer(), 0, 0, 255, 0);
  SDL_Rect collisionRect;

  collisionRect.x = positionX;
  collisionRect.y = positionY + (720 - height);
  collisionRect.w = width;
  collisionRect.h = height;

  SDL_RenderDrawRect(graphics->getRenderer(), &collisionRect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}


const std::map<std::string, CollisionBox::CollisionType> CollisionBox::collisionTypeMap = {
  {"POSITION", CollisionBox::POSITION},
  {"HURT", CollisionBox::HURT},
};
