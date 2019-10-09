#include "domain_language/Stack.h"

void Stack::push(uint8_t value){
  stack[++stackPointer] = value;
};

uint8_t Stack::pop(){
  return stack[stackPointer--];
};

uint8_t Stack::peek(){
  return stack[stackPointer];
};

uint8_t Stack::peekIndex(int index){
  return stack[index];
};
