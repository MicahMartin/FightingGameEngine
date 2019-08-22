#ifndef _StateController_h
#define _StateController_h

#include <string>

class StateController {
public:
  StateController(std::string condition, std::string action);
  ~StateController();

  std::string getCondition();
  std::string getAction();
private:
  std::string condition;
  std::string action;
};
#endif
