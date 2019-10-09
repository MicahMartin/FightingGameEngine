#ifndef _VirtualMachine_h
#define _VirtualMachine_h

#include <stdio.h>
#include <inttypes.h>
#include "Stack.h"
#include "ByteCode.h"

class Character;
class VirtualMachine {
public:
  VirtualMachine(Character* character)
    :character(character){};
  ~VirtualMachine(){};

  void execute(uint8_t bytecode[], int size, int main);

private:
  Stack stack;
  Character* character;
  int globals[256];
  int instructionPointer = 0;
};

#endif
