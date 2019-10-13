#include "domain_language/VirtualMachine.h"
#include "game_objects/Character.h"
#include <bitset>

void VirtualMachine::execute(uint8_t* bytecode, int size, int main) {
  instructionPointer = main;

  while(instructionPointer < size){
    uint8_t instruction = bytecode[instructionPointer++];

    switch (instruction) {
      case PUSH: {
        stack.push(bytecode[instructionPointer++]);
        break;
      }
      case POP: {
        uint8_t val = stack.pop();
        break;
      }
      case ADD: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal+secondVal);
        break;
      }
      case SUB: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal-secondVal);
        break;
      }
      case MUL: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal*secondVal);
        break;
      }
      case CALL: {
        uint8_t address = bytecode[instructionPointer++];
        uint8_t numArgs = bytecode[instructionPointer++];

        stack.push(numArgs);
        stack.push(stack.framePointer);
        stack.push(instructionPointer);

        stack.framePointer = stack.stackPointer;
        instructionPointer = address;
        break;
      }
      case RETURN: {
        uint8_t returnVal = stack.pop();
        stack.stackPointer = stack.framePointer;

        instructionPointer = stack.pop();
        stack.framePointer = stack.pop();
        int numArgs = stack.pop();
        stack.stackPointer -= numArgs;
        stack.push(returnVal);
        break;
      }
      case STORE: {
        break;
      }
      case LOAD: {
        uint8_t offset = bytecode[instructionPointer++];
        stack.push(stack.peekIndex(stack.framePointer + offset));
        break;
      }
      case GSTORE: {
        uint8_t operand = bytecode[instructionPointer++];
        globals[operand] = stack.pop();
        break;
      }
      case GLOAD: {
        uint8_t operand = bytecode[instructionPointer++];
        uint8_t val = globals[operand];

        stack.push(val);
        break;
      }
      case EQUAL: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal == secondVal ? true : false);
        break;
      }
      case NEQUAL: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal != secondVal ? true : false);
        break;
      }
      case GREATER: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal > secondVal ? true : false);
        break;
      }
      case AND: {
        uint8_t secondBool = stack.pop();
        uint8_t firstBool = stack.pop();

        stack.push(firstBool && secondBool ? true : false);
        break;
      }
      case LESS: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal < secondVal ? true : false);
        break;
      }
      case BRANCH: {
        uint8_t operand = bytecode[instructionPointer];
        // printf("branching to line %d\n", operand);
        instructionPointer = operand;
        break;
      }
      case BRANCHT: {
        uint8_t operand = bytecode[instructionPointer++];
        uint8_t boolean = stack.pop();
        if(boolean)
          instructionPointer = operand;
        break;
      }
      case BRANCHF: {
        uint8_t operand = bytecode[instructionPointer++];
        uint8_t boolean = stack.pop();
        if(!boolean)
          instructionPointer = operand;
        break;
      }
      case IBRANCH: {
        break;
      }
      case IBRANCHT: {
        break;
      }
      case IBRANCHF: {
        break;
      }
      case PRINT: {
        uint8_t val = stack.peek();
        break;
      }
      case PRINT_NEXT_OPCODE: {
        uint8_t val = bytecode[instructionPointer++];
        uint8_t secondVal = bytecode[instructionPointer++];
        printf("first val should be 44: %d, second val should be 1: %d\n", val, secondVal);
        break;
      }
      case GET_ANIM_TIME: {
        uint8_t val = character->_getAnimTime();
        stack.push(val);
        break;
      }
      case GET_HIT_STUN: {
        uint8_t val = character->_getHitStun();
        stack.push(val);
        break;
      }
      case GET_STATE_TIME: {
        uint8_t val = character->_getStateTime();
        stack.push(val);
        break;
      }
      case GET_Y_POS: {
        uint8_t val = character->_getYPos();
        stack.push(val);
        break;
      }
      case GET_INPUT: {
        uint8_t operand = bytecode[instructionPointer++];
        uint8_t boolean = character->_getInput(operand);
        stack.push(boolean);
        break;
      }
      case GET_STATE_NUM: {
        uint8_t val = character->_getStateNum();
        stack.push(val);
        break;
      }
      case GET_CONTROL: {
        uint8_t val = character->_getControl();
        stack.push(val);
        break;
      }
      case WAS_PRESSED: {
        uint8_t operand = bytecode[instructionPointer++];
        uint8_t boolean = character->_wasPressed(operand);
        stack.push(boolean);
        break;
      }
      case GET_COMBO: {
        uint8_t val = character->_getCombo();
        stack.push(val);
        break;
      }
      case HAS_AIR_ACTION: {
        stack.push(character->_getAirActions());
        break;
      }
      case CHANGE_STATE: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_changeState(operand);
        // STATE IS DONE EXECUTING. WE GOT UP OUTA THERE.
        return;
      }
      case CANCEL_STATE: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_cancelState(operand);
        // STATE IS DONE EXECUTING. WE GOT UP OUTA THERE.
        return;
      }
      case VELSET_X: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_velSetX(operand);
        break;
      }
      // nhollman json interprets all ints as unsigned so no negatives 
      case NEG_VELSET_X: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_negVelSetX(operand);
        break;
      }
      case VELSET_Y: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_velSetY(operand);
        break;
      }
      case MOVE_F: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_moveForward(operand);
        break;
      }
      case MOVE_B: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_moveBack(operand);
        break;
      }
      case MOVE_U: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_moveUp(operand);
        break;
      }
      case MOVE_D: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_moveDown(operand);
        break;
      }
      case SET_CONTROL: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_setControl(operand);
        break;
      }
      case SET_COMBO: {
        uint8_t operand = bytecode[instructionPointer++];
        character->_setCombo(operand);
        break;
      }
      case SET_GRAVITY: {
        bool operand = bytecode[instructionPointer++];
        character->_setGravity(operand);
        break;
      }
      case SET_NOGRAV_COUNT: {
        int operand = bytecode[instructionPointer++];
        printf("setting noGravCount to %d\n", operand);
        character->_setNoGravityCounter(operand);
        break;
      }
      case SET_AIR_ACTION: {
        bool operand = bytecode[instructionPointer++];
        character->_setAirAction(operand);
        break;
      }
      case RESET_ANIM: {
        character->_resetAnim();
        break;
      }
      case CHECK_COMMAND: {
        uint8_t operand = bytecode[instructionPointer++];
        bool commandFound = character->_checkCommand(operand);
        stack.push(commandFound);
        break;
      }
      case NOP: {
        break;
      }
      case HALT: {
        printf("halting\n");
        return;
      }
      default: {
       printf("invalid opcode\n");
       break;
     }
    }
  }
}
