#ifndef _CommandCompiler_h
#define _CommandCompiler_h

#include <string>
#include <vector>
#include "input/CommandScanner.h"
#include "input/VirtualController.h"
#include <functional>

typedef std::function<bool(int, bool)> CommandFunction;
struct CommandNode {
  CommandFunction function;
  int bufferLength;
};
struct CommandObj {
  std::vector<CommandNode> command;
  bool clears;
};

class CommandCompiler {
public:

  CommandCompiler();
  ~CommandCompiler();

  void init();
  void compile(const char* inputString);

  CommandNode compileNode();
  CommandFunction binaryCommand(CommandFunction currentFunc, CommandTokenType type);

  static std::vector<std::string> commandStrings;
  std::vector<CommandObj> commands;
  VirtualController* controllerPointer;
private:
  CommandScanner commandScanner;
  CommandToken* currentToken;
};

#endif /* _CommandCompiler_h */
