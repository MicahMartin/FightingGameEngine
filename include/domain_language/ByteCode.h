#ifndef _ByteCode_h
#define _ByteCode_h

#include <vector>
#include <string>
#include <map>

enum Instruction { 
  NOP,
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
  PRINT, 
  GET_ANIM_TIME, // game functions
  GET_STATE_TIME,
  GET_Y_POS,
  GET_INPUT,
  GET_STATE_NUM,
  GET_CONTROL,
  WAS_PRESSED,
  GET_COMBO,
  CHANGE_STATE,
  VELSET_X,
  VELSET_Y,
  MOVE_F,
  MOVE_B,
  MOVE_U,
  MOVE_D,
  SET_CONTROL,
  SET_COMBO,
  HALT,
};
static std::map<std::string, Instruction> instructonStrings = {
  { "NOP", NOP },
  { "PUSH", PUSH },
  { "POP", POP },
  { "STORE", STORE },
  { "LOAD", LOAD },
  { "GSTORE", GSTORE },
  { "GLOAD", GLOAD },
  { "ADD", ADD },
  { "SUB", SUB },
  { "MUL", MUL },
  { "EQUAL", EQUAL },
  { "LESS", LESS },
  { "GREATER", GREATER },
  { "BRANCH", BRANCH },
  { "BRANCHT", BRANCHT },
  { "BRANCHF", BRANCHF },
  { "CALL", CALL },
  { "RETURN", RETURN },
  { "PRINT", PRINT },
  { "GET_ANIM_TIME", GET_ANIM_TIME },
  { "GET_STATE_TIME", GET_STATE_TIME },
  { "GET_Y_POS", GET_Y_POS },
  { "GET_INPUT", GET_INPUT },
  { "GET_STATE_NUM", GET_STATE_NUM },
  { "GET_CONTROL", GET_CONTROL },
  { "WAS_PRESSED", WAS_PRESSED },
  { "GET_COMBO", GET_COMBO },
  { "CHANGE_STATE", CHANGE_STATE },
  { "VELSET_X", VELSET_X },
  { "VELSET_Y", VELSET_Y },
  { "MOVE_F", MOVE_F },
  { "MOVE_B", MOVE_B },
  { "MOVE_U", MOVE_U },
  { "MOVE_D", MOVE_D },
  { "SET_CONTROL", SET_CONTROL },
  { "GET_COMBO", SET_COMBO },
  { "HALT", HALT },
};
#endif
