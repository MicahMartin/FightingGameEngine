#ifndef _StateDef_h
#define _StateDef_h

#include <vector>
#include "controllers/StateController.h"

class StateDef {
public:
  StateDef();
  ~StateDef();

private:
  int STATE_NUMBER;
  std::vector<StateController> controllers;
};
#endif
