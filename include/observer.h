#ifndef _observer_h
#define _observer_h


// The messenger, whoever called notify() and notified their observers
// messageType is just a string of the message. I'll have a Message struct soon
template<typename M, typename T>

// observer can observe many subjects
// subject can have many observers
class Observer{
public:
  virtual void onNotify(M messenger, T messageType) = 0;
};
#endif /* ifndef _observer_h */
