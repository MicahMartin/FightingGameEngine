#ifndef _Camera_h
#define _Camera_h

#include "Graphics.h"

struct CameraStateObj{
  int lowerBound = 0;
  int upperBound = 0;
  int middle = 0;
  int yPos = 0;
  SDL_Rect camRect;
};

class Camera {
public:
  Camera();
  ~Camera();

  void init(int width, int height, int screenWidth);
  void update(int p1Xpos, int p2Xpos);
  void render();
  void moveCamera();

  CameraStateObj saveState();
  void loadState(CameraStateObj stateObj);

  SDL_Rect cameraRect;
  int screenWidth = 0;
  int lowerBound = 0;
  int upperBound = 0;
  int middle = 0;
  int yPos = 0;
private:
  Graphics* graphics = Graphics::getInstance();
};

#endif /* _Camera_h */
