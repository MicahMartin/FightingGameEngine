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
  IBRANCH, // branching
  IBRANCHT,
  IBRANCHF,
  CALL, // functions
  RETURN,
  PRINT, 
  PRINT_NEXT_OPCODE, 
  GET_ANIM_TIME, // game functions
  GET_STATE_TIME,
  GET_HIT_STUN,
  GET_Y_POS,
  GET_INPUT,
  GET_STATE_NUM,
  GET_CONTROL,
  WAS_PRESSED,
  GET_COMBO,
  HAS_AIR_ACTION,
  SET_AIR_ACTION,
  CHANGE_STATE,
  CANCEL_STATE,
  VELSET_X,
  NEG_VELSET_X,
  VELSET_Y,
  MOVE_F,
  MOVE_B,
  MOVE_U,
  MOVE_D,
  SET_CONTROL,
  SET_COMBO,
  SET_GRAVITY,
  SET_NOGRAV_COUNT,
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
  { "IBRANCH", IBRANCH },
  { "IBRANCHT", IBRANCHT },
  { "IBRANCHF", IBRANCHF },
  { "CALL", CALL },
  { "RETURN", RETURN },
  { "PRINT", PRINT },
  { "PRINT_NEXT_OPCODE", PRINT_NEXT_OPCODE},
  { "GET_ANIM_TIME", GET_ANIM_TIME },
  { "GET_STATE_TIME", GET_STATE_TIME },
  { "GET_Y_POS", GET_Y_POS },
  { "GET_INPUT", GET_INPUT },
  { "GET_STATE_NUM", GET_STATE_NUM },
  { "GET_CONTROL", GET_CONTROL },
  { "WAS_PRESSED", WAS_PRESSED },
  { "HAS_AIR_ACTION", HAS_AIR_ACTION},
  { "GET_HIT_STUN", GET_HIT_STUN},
  { "CHANGE_STATE", CHANGE_STATE },
  { "CANCEL_STATE", CANCEL_STATE },
  { "VELSET_X", VELSET_X },
  { "NEG_VELSET_X", NEG_VELSET_X },
  { "VELSET_Y", VELSET_Y },
  { "MOVE_F", MOVE_F },
  { "MOVE_B", MOVE_B },
  { "MOVE_U", MOVE_U },
  { "MOVE_D", MOVE_D },
  { "SET_CONTROL", SET_CONTROL },
  { "SET_COMBO", SET_COMBO },
  { "SET_GRAVITY", SET_GRAVITY},
  { "SET_NOGRAV_COUNT", SET_NOGRAV_COUNT},
  { "SET_AIR_ACTION", SET_AIR_ACTION},
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
        uint32_t val = i.value();
        uint8_t *bytes;

        if(val>255){
          memcpy(bytes, &val, sizeof(uint32_t));
          printf("them bytes [%d - %d - %d - %d]\n", bytes[0], bytes[1], bytes[2], bytes[3]);
        } else {
          instruction = i.value();
          byteCode.push_back(instruction);
          printf("pushing back instruction %d\n", instruction);
        }
      } else if (type == nlohmann::json::value_t::string) {
        instruction = instructonStrings[i.value()];
        printf("pushing back instruction %d from string %s!\n", instruction, i.value().get<std::string>().c_str());
        byteCode.push_back(instruction);
      } else {
        printf("unknown type!\n");
      }
    }

    return byteCode;
  };

  static uint32_t littleEndByteToInt(uint8_t byteArray[], int offset) {
    uint32_t result = 0;
    for (int i = offset; i < sizeof(uint32_t); ++i)
        result = (result << CHAR_BIT) + byteArray[i];
    return result;
  }

  static uint32_t bigEndByteToInt(uint8_t byteArray[], int offset) {
      uint32_t result = 0;
      for (int i = offset; i < sizeof(uint32_t); ++i)
          result += byteArray[i] << (i & CHAR_BIT);
      return result;
  }

  static void printbits(int x) {
  for(int i=sizeof(x)<<3; i; i--)
    putchar('0'+((x>>(i-1))&1));
  }
}
#endif
