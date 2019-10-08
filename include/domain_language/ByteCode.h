#ifndef _ByteCode_h
#define _ByteCode_h

enum Instruction { 
  _NULL,
  PUSH, // stack
  POP,
  STORE, // data
  LOAD,
  GSTORE,
  GLOAD,
  ADD, // math
  SUB,
  MUL,
  EQUAL, // comparison
  LESS,
  GREATER,
  BRANCH, // branching
  BRANCHT,
  BRANCHF,
  CALL, // functions
  RETURN,
  PRINT, // misc
  HALT,
};

static const char* InstructionStrings[20] = {
  "NULL",
  "PUSH", // stack
  "POP",
  "STORE", // data
  "LOAD",
  "GSTORE",
  "GLOAD",
  "ADD", // math
  "SUB",
  "MUL",
  "EQUAL", // comparison
  "LESS",
  "GREATER",
  "BRANCH", // branching
  "BRANCH_T",
  "BRANCH_F",
  "CALL", // functions
  "RETURN",
  "PRINT", // misc
  "HALT",
};

#endif
