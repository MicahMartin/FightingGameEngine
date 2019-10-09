#include "domain_language/VirtualMachine.h"

void VirtualMachine::execute(uint8_t bytecode[], int size, int main) {
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
        printf("adding %d+%d\n", firstVal, secondVal);

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
        printf("loading from location %d\n", operand);

        stack.push(val);
        break;
      }
      case EQUAL: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        stack.push(firstVal == secondVal ? true : false);
        break;
      }
      case GREATER: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();

        printf("is %d greater than %d\n", firstVal, secondVal);
        stack.push(firstVal > secondVal ? true : false);
        break;
      }
      case LESS: {
        uint8_t secondVal = stack.pop();
        uint8_t firstVal = stack.pop();
        printf("comparing %d < %d\n", firstVal, secondVal);

        stack.push(firstVal < secondVal ? true : false);
        break;
      }
      case BRANCH: {
        uint8_t operand = bytecode[instructionPointer];
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
      case PRINT: {
        uint8_t val = stack.peek();
        printf("%d\n", val);
        break;
      }
      case NOP: {
        break;
      }
      case HALT: {
        printf("Halting\n");
        return;
      }
    }
  }
}
