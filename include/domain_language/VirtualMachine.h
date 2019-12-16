#ifndef _VirtualMachine_h
#define _VirtualMachine_h

#include "domain_language/Common.h"
#include "domain_language/Script.h"
#include "domain_language/Stack.h"
#include "domain_language/Compiler.h"
#include <unordered_map>

typedef enum {
  EC_OK,
  EC_COMPILE_ERROR,
  EC_RUNTIME_ERROR,
} ExecutionCode;


class GameObject;
class VirtualMachine {
public:
  VirtualMachine();
  ~VirtualMachine();

  ExecutionCode execute(Script* script);
  bool debugMode = false;

  Compiler compiler;
  GameObject* character;
private:
  ExecutionCode run();
  void runtimeError(const char* format, ...);
  bool isFalsey(Value value);
  bool valuesEqual(Value valueA, Value valueB);
  void concatenate();

  Script* scriptPointer;
  uint8_t* instructionPointer;
  Stack stack;
  //TODO: Free these objects
};

#endif
