#ifndef _StateDef_h
#define _StateDef_h

#include <unordered_set>
#include "controllers/StateController.h"

class StateDef {
public:
  StateDef();
  ~StateDef();

private:
  int STATE_NUMBER;
  std::unordered_set<StateController> controllers;
};
#endif
