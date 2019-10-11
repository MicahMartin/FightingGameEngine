#ifndef _ByteCode_h
#define _ByteCode_h

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <json.hpp>

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
  NEQUAL,
  LESS,
  GREATER,
  AND,
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
  NEG_VELSET_X,
  VELSET_Y,
  MOVE_F,
  MOVE_B,
  MOVE_U,
  MOVE_D,
  SET_CONTROL,
  SET_COMBO,
  RESET_ANIM,
  CHECK_COMMAND,
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
  { "NEQUAL", NEQUAL },
  { "LESS", LESS },
  { "GREATER", GREATER },
  { "AND", AND },
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
  { "NEG_VELSET_X", NEG_VELSET_X },
  { "VELSET_Y", VELSET_Y },
  { "MOVE_F", MOVE_F },
  { "MOVE_B", MOVE_B },
  { "MOVE_U", MOVE_U },
  { "MOVE_D", MOVE_D },
  { "SET_CONTROL", SET_CONTROL },
  { "SET_COMBO", SET_COMBO },
  { "RESET_ANIM", RESET_ANIM},
  { "CHECK_COMMAND", CHECK_COMMAND},
  { "HALT", HALT },
};

namespace ByteCode {

  static std::vector<uint8_t> compile(nlohmann::json json){
    std::vector<uint8_t> byteCode;

    for(auto i : json.items()){
      // If the value isn't already a number literal, get the int representation of that instruction string
      uint8_t instruction;
      nlohmann::json::value_t type = i.value().type();
      if(type == nlohmann::json::value_t::number_unsigned){
        instruction = i.value();
      } else if (type == nlohmann::json::value_t::string) {
        instruction = instructonStrings[i.value()];
      }
      byteCode.push_back(instruction);
    }

    return byteCode;
  };
}
#endif
