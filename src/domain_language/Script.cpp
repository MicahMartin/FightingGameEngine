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

