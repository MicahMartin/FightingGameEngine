#include "graphics/Camera.h"

Camera::Camera(){ }

void Camera::init(int width, int height, int _screenWidth){
  screenWidth = _screenWidth;
  positionObj.w = width;
  positionObj.h = height;
  positionObj.y = 0;

  cameraRect.w = width/100;
  cameraRect.h = height/100;
  cameraRect.y = 0;
}

CameraStateObj Camera::saveState(){
  CameraStateObj stateObj;
  stateObj.camRect = cameraRect;
  stateObj.lowerBound = lowerBound;
  stateObj.upperBound = upperBound;
  stateObj.middle = middle;

  return stateObj;
}

void Camera::loadState(CameraStateObj stateObj){
  cameraRect = stateObj.camRect;
  lowerBound = stateObj.lowerBound;
  upperBound = stateObj.upperBound;
  middle = stateObj.middle;
}

Camera::~Camera(){}

void Camera::update(std::pair<int,int> p1Pos, std::pair<int,int> p2Pos){
  int midPoint = (p1Pos.first + p2Pos.first) / 2;
  positionObj.x = (midPoint - (positionObj.w/2));
  if(positionObj.x < 0){
    positionObj.x = 0;
  }
  if(positionObj.x + positionObj.w > screenWidth){
    positionObj.x = screenWidth - positionObj.w;
  }

  lowerBound = positionObj.x;
  upperBound = positionObj.x+positionObj.w;
  middle = positionObj.x + (positionObj.w/2);

  int highest = p1Pos.second > p2Pos.second ? p1Pos.second : p2Pos.second;
  if(highest > (graphics->getWindowHeight()/2)){
    positionObj.y = highest - (graphics->getWindowHeight() / 2);
  } else {
    positionObj.y = 0;
  }
  
  //TODO: COORDINATE_SCALE
  cameraRect.x = positionObj.x/100;
  cameraRect.y = positionObj.y/100;
}

void Camera::render(){
  SDL_SetRenderDrawColor(graphics->getRenderer(), 255, 0, 0, 0);
  SDL_RenderDrawRect(graphics->getRenderer(), &cameraRect);
  SDL_SetRenderDrawColor(graphics->getRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
}
