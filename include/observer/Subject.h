#ifndef _Subject_h
#define _Subject_h

#include "Observer.h"

// The messenger, whoever called notify() and notified their observers
// observer can observe many subjects
// subject can have many observers
class Subject {
protected:
  virtual void addObserver(const char* observerName, Observer* observer) = 0;
  virtual void removeObserver(const char* observerName) = 0;
  virtual void notifyAll(const char* eventName) = 0;
  virtual void notifyOne(const char* observerName, const char* eventName) = 0;

private:
};
#endif /* ifndef _subject_h */
