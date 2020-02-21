#ifndef _Popup_h
#define _Popup_h

#include <utility>
#include "game_objects/VisualObject.h"
#include "graphics/Animation.h"

class Popup : public VisualObject {
public:
  Popup();
  ~Popup();

  void update();
  void draw();

  void loadDataFile(const char* filePath);

  std::pair<int,int> getPos();
  void setX(int x);
  void setY(int y);
  int getX();
  int getY();
  
  void setActive(bool active);
  bool getActive();

  void incrementStateTime();
  void setStateTime(int newStateTime);
  int getStateTime();

private:
  bool isActive = false;
  int stateTime = 0;
  int xPos = 0;
  int yPos = 0;
  int playLength = 0;
  Animation anim;
};

#endif

