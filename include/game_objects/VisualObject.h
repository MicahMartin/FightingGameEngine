#ifndef _VisualObject_h
#define _VisualObject_h

#include <utility>

class VisualObject {
public:
  virtual ~VisualObject() {};

  virtual void update() = 0;
  virtual void draw() = 0;

  virtual void loadDataFile(const char* filePath) = 0;

  virtual std::pair<int,int> getPos() = 0;
  virtual void setX(int x) = 0;
  virtual void setY(int y) = 0;
  virtual int getX() = 0;
  virtual int getY() = 0;

  virtual void setActive(bool active) = 0;
  virtual bool getActive() = 0;

  virtual void incrementStateTime() = 0;
  virtual void setStateTime(int stateTime) = 0;
  virtual int getStateTime() = 0;
  
private:
};

#endif /* _VisualObject_h */
