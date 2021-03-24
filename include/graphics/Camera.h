#ifndef _Camera_h
#define _Camera_h

#include "Graphics.h"

struct CamPosition {
  int x,
      y,
      w,
      h,
      upperBound,
      lowerBound,
      middle;
};

struct CameraStateObj {
  int lowerBound = 0;
  int upperBound = 0;
  int middle = 0;
  SDL_Rect camRect;
};

class Camera {
public:
  Camera();
  ~Camera();

  void init(int width, int height, int screenWidth);
  void update(std::pair<int,int> p1Pos, std::pair<int,int> p2Pos);

  void startShake(float radius);
  void render();

  CameraStateObj saveState();
  void loadState(CameraStateObj stateObj);

  CamPosition positionObj;
  SDL_Rect cameraRect;
  int screenWidth = 0;
  int lowerBound = 0;
  int upperBound = 0;
  int middle = 0;
  int shakeCount = 0;
  int zoomThresh = 0;
  float zoomMag = 1;
  bool shaking = false;

private:
  Graphics* graphics = Graphics::getInstance();
  int viewportCentreX;
  int viewportCentreY;
  int radius;
  int randomAngle;
  int offsetX;
  int offsetY;
};

#endif /* _Camera_h */
