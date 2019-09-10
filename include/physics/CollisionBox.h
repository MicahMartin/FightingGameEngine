#ifndef _CollisionBox_h
#define _CollisionBox_h

#include <map>
class CollisionBox {
public:
  enum CollisionType {
    POSITION,
    HURT
  };

  std::map<std::string, int> map = {
    {"FOO", 1}
  };

  CollisionBox(CollisionType boxType);
  ~CollisionBox();
  
private:
  CollisionType boxType;
};

#endif
