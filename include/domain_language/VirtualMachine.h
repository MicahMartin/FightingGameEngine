#ifndef _VirtualMachine_h
#define _VirtualMachine_h

#include <stdio.h>
#include <inttypes.h>
#include "Stack.h"
#include "ByteCode.h"

class Character;
class VirtualMachine {
public:
  VirtualMachine() {
    instructionPointer = 0;
  };
  ~VirtualMachine(){};

  void init(Character* charPointer){
    character = charPointer;
  }

  void execute(uint8_t* bytecode, int size, int main);

private:
  Stack stack;
  Character* character;
  int globals[256];
  int instructionPointer;
};

#endif
