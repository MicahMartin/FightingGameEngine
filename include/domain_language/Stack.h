#ifndef _Stack_h
#define _Stack_h

#include <stdio.h>
#include <inttypes.h>

class Stack {
public:
  Stack() {
    stackPointer = -1;
  };
  ~Stack(){};

  void push(uint8_t value);
  uint8_t pop();

  uint8_t peek();
  uint8_t peekIndex(int index);

  int stackPointer;
  int framePointer;
private:
  int stack[256];
};

#endif
