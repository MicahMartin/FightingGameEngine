#ifndef _Observer_h
#define _Observer_h


// The messenger, whoever called notify() and notified their observers
// not sure if this is a proper use of templates its just making sense
// observer can observe many subjects
// subject can have many observers
class Observer{
public:
  virtual void onNotify(const char* eventName) = 0;
};
#endif /* ifndef _observer_h */
