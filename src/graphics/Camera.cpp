#include "graphics/Camera.h"
#include "util/Util.h"

Camera::Camera(){ }

void Camera::init(int width, int height, int _screenWidth){
  screenWidth = _screenWidth;
  positionObj.w = width;
  positionObj.h = height;
  positionObj.y = 0;

  cameraRect.w = width/COORDINATE_SCALE;
  cameraRect.h = height/COORDINATE_SCALE;
  cameraRect.y = 0;
  shaking = false;
  zoomThresh = 750000;
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
  int diff = abs(p1Pos.first - p2Pos.first);
  if (diff > zoomThresh) {
    zoomMag = .5;
  } else {
    zoomMag = 1;
  }
  // printf("thresh:%d the diff: %d, zoomMag:%f\n", zoomThresh, diff, zoomMag);
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

  int highest = p1Pos.second < p2Pos.second ? p1Pos.second : p2Pos.second;
  if(abs(highest) > (positionObj.h/2)){
    positionObj.y = abs(highest) - (positionObj.h/2);
  } else {
    positionObj.y = 0;
  }
  
  cameraRect.x = positionObj.x/COORDINATE_SCALE;
  cameraRect.y = positionObj.y/COORDINATE_SCALE;
}

void Camera::startShake(float _radius){
  shaking = true;
  viewportCentreX = cameraRect.x/2;
  viewportCentreY = cameraRect.y/2;
  radius = _radius;
  randomAngle = rand()%360;
}

void Camera::render(){
  if (shaking) {
    offsetX = sin(randomAngle) * radius;
    offsetY = cos(randomAngle) * radius;
    cameraRect.x = (viewportCentreX+offsetX)*2;
    cameraRect.y = (viewportCentreY+offsetY)*2;
    radius *= .9;
    randomAngle += (150 + rand()%60);
    if (radius <= 2) {
      shaking = false;
    }
  }
}
