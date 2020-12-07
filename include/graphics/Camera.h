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
  void render();
  void moveCamera();

  CameraStateObj saveState();
  void loadState(CameraStateObj stateObj);

  SDL_Rect cameraRect;
  int screenWidth = 0;
  int lowerBound = 0;
  int upperBound = 0;
  int middle = 0;
  CamPosition positionObj;
private:
  Graphics* graphics = Graphics::getInstance();
};

#endif /* _Camera_h */
