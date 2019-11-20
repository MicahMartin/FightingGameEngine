#ifndef _CommandCompiler_h
#define _CommandCompiler_h

#include <string>
#include <vector>
#include "input/CommandScanner.h"
#include "input/VirtualController.h"
#include <functional>

typedef struct {
  CommandToken current;
  CommandToken previous;
} CommandParser;


typedef std::function<bool(int, bool)> CommandFunction;

class CommandCompiler {
public:

  CommandCompiler();
  ~CommandCompiler();

  void init();
  void compile(const char* inputString);
  CommandFunction compileNode();

  void consume(CommandTokenType endType);
  void match(CommandTokenType matchType);
  CommandFunction binaryCommand(CommandFunction currentFunc, CommandTokenType type);

  static std::vector<std::string> commandStrings;
  std::vector<std::vector<CommandFunction>> commandFunctionList;
  VirtualController* controllerPointer;
private:
  CommandScanner commandScanner;
  CommandToken* currentToken;
};

#endif /* _CommandCompiler_h */
