#include "domain_language/VirtualMachine.h"
#include "game_objects/Character.h"


VirtualMachine::VirtualMachine(){}

VirtualMachine::~VirtualMachine(){}

inline ExecutionCode VirtualMachine::run(){
  const char* unreachable = R""""(If you're seeing this, the code is in what I thought was an unreachable state. 
  I could give you advice for what to do. But honestly, why should you trust me? I clearly screwed this up. 
  I'm writing a message that should never appear, yet I know it will probably appear someday.
  On a deep level, I know I'm not up to this task. I'm so sorry.)"""";
  // lets go fast bb
  #define READ_BYTE() (*instructionPointer++)
  #define READ_SYMBOL() (scriptPointer->symbols[READ_BYTE()])
  #define READ_SHORT() \
    (instructionPointer += 2, (uint16_t)((instructionPointer[-2] << 8) | instructionPointer[-1]))
  #define READ_STRING() AS_STRING(READ_SYMBOL())
  #define BINARY_OP(valueType, op) \
    do { \
      if (!IS_NUMBER(stack.peek(0)) || !IS_NUMBER(stack.peek(1))) { \
        runtimeError(unreachable); \
        return EC_RUNTIME_ERROR; \
      } \
      long b = AS_NUMBER(stack.pop()); \
      long a = AS_NUMBER(stack.pop()); \
      stack.push(valueType(a op b)); \
    } while (false)

  for (;;) {
    if (debugMode) { 
      printf("          ");
      for (Value* slot = stack.stack; slot < stack.stackTop; slot++) {
        printf("[ ");
        ValueFn::printValue(*slot);
        printf(" ]");
      }
      printf("\n");
      scriptPointer->disassembleInstruction((int)(instructionPointer - scriptPointer->scriptStart()));
    }

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
        Value symbol = READ_SYMBOL();
        stack.push(symbol);
        break;
      }
      case OP_NIL: stack.push(NIL_VAL); break;
      case OP_TRUE: stack.push(BOOL_VAL(true)); break;
      case OP_FALSE: stack.push(BOOL_VAL(false)); break;
      case OP_POP: stack.pop(); break;
      case OP_GET_LOCAL: {
        uint8_t slot = READ_BYTE();
        Value val = stack.at(slot);
        stack.push(val);
        break;
      }
       case OP_SET_LOCAL: {
        uint8_t slot = READ_BYTE();
        stack.set(stack.peek(0), slot);
        break;
      }
      case OP_GET_GLOBAL: {
        // TODO: No globals
        std::string* name = READ_STRING();
//        for (auto i : scriptPointer->globals) {
//          printf("wtf %s\n", AS_STRING(i.second)->chars);
//        }
        auto globalVal = scriptPointer->globals.find(*name);
        if (globalVal == scriptPointer->globals.end()) {
          runtimeError("Undefined variable '%s'.", name->c_str());
          return EC_RUNTIME_ERROR;
        }
        stack.push(globalVal->second);
        break;
      }
      case OP_DEFINE_GLOBAL: {               
        std::string* name = READ_STRING();
        // printf("defining global %s:%s\n", name->chars, AS_STRING(stack.peek(0))->chars);
        scriptPointer->globals.insert(std::make_pair(*name, stack.peek(0)));
        stack.pop();
        break;                               
      }                                      
      case OP_SET_GLOBAL: {               
        std::string* name = READ_STRING();
        auto globalVal = scriptPointer->globals.find(*name);
        if (globalVal == scriptPointer->globals.end()) {
          runtimeError("Undefined variable '%s'.", name->c_str());
          return EC_RUNTIME_ERROR;
        }
        scriptPointer->globals[*name] = stack.peek(0);
        break;                               
      }                                      
      case OP_EQUAL: {
        Value b = stack.pop();
        Value a = stack.pop();
        stack.push(BOOL_VAL(valuesEqual(a, b)));
        break;
      }
      case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
      case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;
      case OP_ADD: {
        if (IS_STRING(stack.peek(0)) && IS_STRING(stack.peek(1))) {
          concatenate();
        } else if (IS_NUMBER(stack.peek(0)) && IS_NUMBER(stack.peek(1))) {
          long b = AS_NUMBER(stack.pop());
          long a = AS_NUMBER(stack.pop());
          stack.push(NUMBER_VAL(a + b));
        } else {
          runtimeError("Operands must be two numbers or two strings.");
          return EC_RUNTIME_ERROR;
        }
        break;
      }
      case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
      case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
      case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
      case OP_NOT:
        stack.push(BOOL_VAL(isFalsey(stack.pop())));
        break;
      case OP_NEGATE: {
        if (!IS_NUMBER(stack.peek(0))) {
          runtimeError("Operand must be a number.");
          return EC_RUNTIME_ERROR;
        }

        stack.push(NUMBER_VAL(-AS_NUMBER(stack.pop())));
        break;
      }
      case OP_PRINT: {
        ValueFn::printValue(stack.pop());
        printf("\n");
        break;
      }
      case OP_JUMP_IF_FALSE: {
        uint16_t offset = READ_SHORT();
        if (isFalsey(stack.peek(0))) instructionPointer  += offset;
        break;
      }
      case OP_JUMP: {
        uint16_t offset = READ_SHORT();
        instructionPointer += offset;
        break;
      }
      case OP_LOOP: {
        uint16_t offset = READ_SHORT();
        instructionPointer -= offset;
        break;
      }
      case OP_GET_ANIM_TIME: {
        long val = character->_getAnimTime();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_GET_HIT_STUN: {
        long val = character->_getHitStun();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_GET_STATE_TIME: {
        int val = character->_getStateTime();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_GET_Y_POS: {
        long val = character->_getYPos();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_GET_INPUT: {
        long operand = AS_NUMBER(stack.pop());
        bool boolean = character->_getInput(operand);
        stack.push(BOOL_VAL(boolean));
        break;
      }
      case OP_GET_STATE_NUM: {
        long val = character->_getStateNum();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_GET_CONTROL: {
        long val = character->_getControl();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_WAS_PRESSED: {
        long operand = AS_NUMBER(stack.pop());
        bool boolean = character->_wasPressed(operand);
        stack.push(BOOL_VAL(boolean));
        break;
      }
      case OP_GET_COMBO: {
        long val = character->_getCombo();
        stack.push(NUMBER_VAL(val));
        break;
      }
      case OP_HAS_AIR_ACTION: {
        stack.push(NUMBER_VAL(character->_getAirActions()));
        break;
      }
      case OP_CHANGE_STATE: {
        long operand = AS_NUMBER(stack.pop());
        character->_changeState(operand);
        // STATE IS DONE EXECUTING. WE GOT UP OUTA THERE.
        return EC_OK;
      }
      case OP_CANCEL_STATE: {
        long operand = AS_NUMBER(stack.pop());
        character->_cancelState(operand);
        // STATE IS DONE EXECUTING. WE GOT UP OUTA THERE.
        return EC_OK;
      }
      case OP_VELSET_X: {
        long operand = AS_NUMBER(stack.pop());
        character->_velSetX(operand);
        break;
      }
      // this gets called often enough to justify its own instruction rather than negating the val
      case OP_NEG_VELSET_X: {
        long operand = AS_NUMBER(stack.pop());
        character->_negVelSetX(operand);
        break;
      }
      case OP_VELSET_Y: {
        long operand = AS_NUMBER(stack.pop());
        character->_velSetY(operand);
        break;
      }
      case OP_MOVE_F: {
        long operand = AS_NUMBER(stack.pop());
        character->_moveForward(operand);
        break;
      }
      case OP_MOVE_B: {
        long operand = AS_NUMBER(stack.pop());
        character->_moveBack(operand);
        break;
      }
      case OP_MOVE_U: {
        long operand = AS_NUMBER(stack.pop());
        character->_moveUp(operand);
        break;
      }
      case OP_MOVE_D: {
        long operand = AS_NUMBER(stack.pop());
        character->_moveDown(operand);
        break;
      }
      case OP_SET_CONTROL: {
        long operand = AS_NUMBER(stack.pop());
        character->_setControl(operand);
        break;
      }
      case OP_SET_COMBO: {
        long operand = AS_NUMBER(stack.pop());
        character->_setCombo(operand);
        break;
      }
      case OP_SET_GRAVITY: {
        bool operand = READ_BYTE();
        character->_setGravity(operand);
        break;
      }
      case OP_SET_NOGRAV_COUNT: {
        int operand = READ_BYTE();
        printf("setting noGravCount to %d\n", operand);
        character->_setNoGravityCounter(operand);
        break;
      }
      case OP_SET_AIR_ACTION: {
        bool operand = READ_BYTE();
        character->_setAirAction(operand);
        break;
      }
      case OP_RESET_ANIM: {
        character->_resetAnim();
        break;
      }
      case OP_CHECK_COMMAND: {
        long operand = AS_NUMBER(stack.pop());
        bool commandFound = character->_checkCommand(operand);
        stack.push(BOOL_VAL(commandFound));
        break;
      }
      case OP_RETURN: {
        return EC_OK;
      }
      default: {
       printf("invalid opcode\n");
       break;
     }
    }
  }

  #undef READ_BYTE
  #undef READ_STRING
  #undef READ_SYMBOL
  #undef READ_SHORT
  #undef BINARY_OP
}

ExecutionCode VirtualMachine::execute(Script* script){
  stack.reset();
  scriptPointer = script;
  instructionPointer = scriptPointer->scriptStart();

  ExecutionCode result = run();
  // TODO: account for constant table / string table
  return EC_OK;
};

void VirtualMachine::runtimeError(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = instructionPointer - scriptPointer->scriptStart();
  int line = scriptPointer->lines[instruction];
  fprintf(stderr, "[line %d] in script\n", line);

  stack.reset();
}

inline bool VirtualMachine::isFalsey(Value value) {
  return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

inline void VirtualMachine::concatenate() {
  std::string* b = AS_STRING(stack.pop());
  std::string* a = AS_STRING(stack.pop());
  // TODO: intern for garbage collection
  std::string* newString = new std::string(*a + *b);
  stack.push(STRING_VAL(newString));
}

inline bool VirtualMachine::valuesEqual(Value valueA, Value valueB) {
  if (valueA.type != valueB.type) return false;

  switch (valueA.type) {
    case VAL_BOOL:   return AS_BOOL(valueA) == AS_BOOL(valueB);
    case VAL_NIL:    return true;
    case VAL_NUMBER: return AS_NUMBER(valueA) == AS_NUMBER(valueB);
    case VAL_STRING: return  *(AS_STRING(valueA)) == *(AS_STRING(valueB));
  }
}

