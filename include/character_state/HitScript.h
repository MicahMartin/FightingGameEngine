#ifndef _HitScript_h 
#define _HitScript_h

#include <nlohmann/json.hpp>

class HitScript {
public:
  HitScript(int scriptNum, int screenFreeze, int damage);
  ~HitScript();

  int scriptNum;
  int screenFreeze;
  int damage;
private:
};

#endif 
