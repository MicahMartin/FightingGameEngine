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
typedef std::vector<CommandFunction> Command;

class CommandCompiler {
public:

  CommandCompiler();
  ~CommandCompiler();

  void init();
  void compile(const char* inputString);

  CommandFunction compileNode();
  CommandFunction binaryCommand(CommandFunction currentFunc, CommandTokenType type);

  static std::vector<std::string> commandStrings;
  std::vector<Command> commands;
  VirtualController* controllerPointer;
private:
  CommandScanner commandScanner;
  CommandToken* currentToken;
};

#endif /* _CommandCompiler_h */
