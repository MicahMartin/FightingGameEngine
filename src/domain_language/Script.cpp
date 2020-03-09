#include "domain_language/Script.h"

Script::Script(){
  code.reserve(8);
}

Script::~Script(){}

void Script::writeByte(uint8_t byte, int lineNumber){
  code.push_back(byte);
  lines.push_back(lineNumber);
}

uint8_t Script::writeSymbol(Value value){
  symbols.push_back(value);
  return symbols.size() - 1;
}

uint8_t* Script::scriptStart(){
  return &code[0];
}
/************************************** debugging *************************************/
// TODO - template based debuggre
int Script::jumpInstruction(const char* name, int sign, int offset) {                              
  uint16_t jump = (uint16_t)(code[offset + 1] << 8);           
  jump |= code[offset + 2];                                    
  printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
  return offset + 3;                                                  
}

int Script::byteInstruction(const char* name, int offset) {
  uint8_t slot = code[offset + 1];
  printf("%-16s %4d\n", name, slot);
  return offset + 2;
}

int Script::symbolInstruction(const char* name, int offset) {
  // get operand (symbol index)
  uint8_t symbolIndex = code[++offset];
  printf("%-16s %4d '", name, symbolIndex);
  // print the symbol
  Value symbol = symbols[symbolIndex];
  ValueFn::printValue(symbol);
  printf("'\n");

  // skip the operand
  return ++offset;
}

int Script::simpleInstruction(const char* name, int offset) {
  printf("%s\n", name);
  return ++offset;
}

int Script::disassembleInstruction(int offset){
  printf("%04d ", offset);
  if (offset > 0 && lines[offset] == lines[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", lines[offset]);
  }

  uint8_t instruction = code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return symbolInstruction("OP_CONSTANT", offset);
      break;
    case OP_NIL:
      return simpleInstruction("OP_NIL", offset);
    case OP_TRUE:
      return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
      return simpleInstruction("OP_FALSE", offset);
    case OP_POP:
      return simpleInstruction("OP_POP", offset);
    case OP_DEFINE_GLOBAL:
      return symbolInstruction("OP_DEFINE_GLOBAL", offset);
    case OP_GET_LOCAL:
      return byteInstruction("OP_GET_LOCAL", offset);
    case OP_SET_LOCAL:
      return byteInstruction("OP_SET_LOCAL", offset);
    case OP_GET_GLOBAL:
      return symbolInstruction("OP_GET_GLOBAL", offset);
    case OP_SET_GLOBAL:
      return symbolInstruction("OP_SET_GLOBAL", offset);
    case OP_EQUAL:
      return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
      return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
      return simpleInstruction("OP_LESS", offset);
    case OP_ADD:
      return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
      return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
      return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
      return simpleInstruction("OP_DIVIDE", offset);
    case OP_NOT:
      return simpleInstruction("OP_NOT", offset);
    case OP_NEGATE:
      return simpleInstruction("OP_NEGATE", offset);
    case OP_PRINT:
      return simpleInstruction("OP_PRINT", offset);
    case OP_JUMP:
      return jumpInstruction("OP_JUMP", 1, offset);
    case OP_JUMP_IF_FALSE:
      return jumpInstruction("OP_JUMP_IF_FALSE", 1, offset);
    case OP_LOOP:
      return jumpInstruction("OP_LOOP", -1, offset);
    case OP_GET_ANIM_TIME: 
      return simpleInstruction("OP_GET_ANIM_TIME", offset);
    case OP_GET_HIT_STUN: 
      return simpleInstruction("OP_GET_HIT_STUN", offset);
    case OP_GET_BLOCK_STUN: 
      return simpleInstruction("OP_GET_BLOCK_STUN", offset);
    case OP_GET_STATE_TIME: 
      return simpleInstruction("OP_GET_STATE_TIME", offset);
    case OP_GET_Y_POS: 
      return simpleInstruction("OP_GET_Y_POS", offset);
    case OP_GET_INPUT: 
      return simpleInstruction("OP_GET_INPUT", offset);
    case OP_GET_STATE_NUM: 
      return simpleInstruction("OP_GET_STATE_NUM", offset);
    case OP_GET_CONTROL: 
      return simpleInstruction("OP_GET_CONTROL", offset);
    case OP_GET_HIT_CANCEL: 
      return simpleInstruction("OP_GET_HIT_CANCEL", offset);
    case OP_GET_WHIFF_CANCEL: 
      return simpleInstruction("OP_GET_WHIFF_CANCEL", offset);
    case OP_WAS_PRESSED: 
      return simpleInstruction("OP_WAS_PRESSED", offset);
    case OP_GET_COMBO: 
      return simpleInstruction("OP_GET_COMBO", offset);
    case OP_GET_IS_ALIVE: 
      return simpleInstruction("OP_GET_IS_ALIVE", offset);
    case OP_HAS_AIR_ACTION: 
      return simpleInstruction("OP_HAS_AIR_ACTION", offset);
    case OP_CHANGE_STATE: 
      return simpleInstruction("OP_CHANGE_STATE", offset);
    case OP_CANCEL_STATE: 
      return simpleInstruction("OP_CANCEL_STATE", offset);
    case OP_VELSET_X: 
      return simpleInstruction("OP_VELSET_X", offset);
    // this gets called often enough to justify its own instruction rather than negating the val
    case OP_NEG_VELSET_X: 
      return simpleInstruction("OP_NEG_VELSET_X", offset);
    case OP_VELSET_Y: 
      return simpleInstruction("OP_VELSET_Y", offset);
    case OP_MOVE_F: 
      return simpleInstruction("OP_MOVE_F", offset);
    case OP_MOVE_B: 
      return simpleInstruction("OP_MOVE_B", offset);
    case OP_MOVE_U: 
      return simpleInstruction("OP_MOVE_U", offset);
    case OP_MOVE_D: 
      return simpleInstruction("OP_MOVE_D", offset);
    case OP_SET_CONTROL:
      return simpleInstruction("OP_SET_CONTROL", offset);
    case OP_SET_COMBO:
      return simpleInstruction("OP_SET_COMBO", offset);
    case OP_SET_GRAVITY:
      return simpleInstruction("OP_SET_GRAVITY", offset);
    case OP_SET_NOGRAV_COUNT:
      return simpleInstruction("OP_SET_NOGRAV_COUNT", offset);
    case OP_SET_AIR_ACTION:
      return simpleInstruction("OP_SET_AIR_ACTION", offset);
    case OP_SET_HIT_STUN:
      return simpleInstruction("OP_SET_HIT_STUN", offset);
    case OP_RESET_ANIM:
      return simpleInstruction("OP_RESET_ANIM", offset);
    case OP_ACTIVATE_ENTITY:
      return simpleInstruction("OP_ACTIVATE_ENTITY", offset);
    case OP_DEACTIVATE_ENTITY:
      return simpleInstruction("OP_DEACTIVATE_ENTITY", offset);
    case OP_SNAP_TO_OPPONENT:
      return simpleInstruction("OP_SNAP_TO_OPPONENT", offset);
    case OP_CHECK_COMMAND:
      return simpleInstruction("OP_CHECK_COMMAND", offset);
    case OP_SET_HIT_CANCEL:
      return simpleInstruction("OP_SET_HIT_CANCEL", offset);
    case OP_SET_WHIFF_CANCEL:
      return simpleInstruction("OP_SET_WHIFF_CANCEL", offset);
    case OP_SET_COUNTER:
      return simpleInstruction("OP_SET_COUNTER", offset);
    case OP_RETURN:
      return simpleInstruction("OP_RETURN", offset);
      break;
    default:
      printf("unknown op code! %d\n", instruction);
      return ++offset;
  }
}

void Script::disassembleScript(const char* name){
  printf("== %s ==\n", name);

  for (int offset = 0; offset < code.size();) {
    offset = disassembleInstruction(offset);
  }
}

