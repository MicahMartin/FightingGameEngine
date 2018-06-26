#ifndef _subject_h
#define _subject_h

#include "observer.h"
#include <vector>

// The messenger, whoever called notify() and notified their observers
// observer can observe many subjects
// subject can have many observers
class Subject{
protected:
  virtual void addObserver(Observer* observer) = 0;
  virtual void removeObserver(Observer* observer) = 0;
  virtual void notifyAll(const char* messageType) = 0;

private:
};
#endif /* ifndef _subject_h */
