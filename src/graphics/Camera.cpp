#include "graphics/Camera.h"

Camera::Camera(){
  cameraRect.y = 0;
  cameraRect.w = graphics->getWindowWidth();
  cameraRect.h = graphics->getWindowHeight();
  cameraRect.x = 0;
  printf("camera itialized %d %d\n", cameraRect.w, cameraRect.h);
}
Camera::~Camera(){}

void Camera::update(int p1Xpos, int p2Xpos){
}

void Camera::moveCamera(){
  cameraRect.x += 10;
}
