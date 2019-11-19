#include "input/CommandCompiler.h"

std::vector<std::string> CommandCompiler::commandStrings = {
  "F, N, F",
  "B, N, B",
  "@~D, N, @D, LP",
  "@F & !D, N, F"
  "@B & !D, N, B",
  "~D, DF, @F & !D, LK | ~LK",
  "~D, DB, @B & !D, LP | ~LP",
  "MP & *F",
  "MP & *B",
};

CommandCompiler::CommandCompiler() {
  for (int i = 0; i < commandStrings.size(); ++i) {
    compile(commandStrings[i].c_str());
  }
}

CommandCompiler::~CommandCompiler(){ }

void CommandCompiler::compile(const char* inputString) {
  std::vector<CommandToken> tokens = commandScanner.scan(inputString);
  printf("the tokenList\n");
  for (auto i : tokens) {
    printf("TOKEN:%s\n", std::string(i.start, i.length).c_str());
  }
}
