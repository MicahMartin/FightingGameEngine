#ifndef _VirtualMachine_h
#define _VirtualMachine_h

#include <stdio.h>
#include <inttypes.h>
#include "Stack.h"
#include "ByteCode.h"

class VirtualMachine {
public:
  VirtualMachine (){};
  ~VirtualMachine(){};

  void execute(uint8_t bytecode[], int size, int main);

private:
  Stack stack;
  int globals[256];
  int instructionPointer = 0;
};

#endif
