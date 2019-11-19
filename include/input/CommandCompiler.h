#ifndef _CommandCompiler_h
#define _CommandCompiler_h

#include <string>
#include <stack>
#include <vector>
#include "input/CommandScanner.h"

// each 'commandString' is a descriptor for a stack of boolean function calls
// P | ~P = ((wasPressed(LP)) || (wasReleased(LP)))
// @F & !D = ((wasPressed(F, strict = false)) && !(wasPressed(D)))
// MP & *D = ((wasPressed(MP)) && (isPressed(F)))
// DF = (wasPressed(DF))
// ~D = (wasReleased(D))
// input = N, F, B, U, D, UF, UB, DF, DB, LP, LK, MP, MK
// pressMod = ~, *
// strictMod = @
// notMod = !
// binary = &, |

//  Forward, neutral, forward
//  "F, N, F",
//  back , neutral, back 
//  "B, N, B",
//  any down release, neutral, any down press, LP
//  "@~D, N, @D, LP",
//  any forward that doesnt include down, neutral, forward (lienent dash)
//  "@F & !D, N, F"
//  any back that doesnt include down, neutral, forward (lienent backdash)
//  "@B & !D, N, B",
//  release of down, downforward, anyforward that doesnt include down, lk or release of lk (236K)
//  "~D, DF, @F & !D, LK | ~LK",
//  release of down, downback, any back that doesnt include down, lp or release of lp (214P)
//  "~D, DB, @B & !D, LP | ~LP",
//  MP + forward IS pressed
//  "MP & *F",
//  MP + back IS pressed 
//  "MP & *B",
typedef bool *(CommandFunction)();

typedef struct {
  CommandToken current;
  CommandToken previous;
} CommandParser;

class CommandCompiler {
public:

  CommandCompiler();
  ~CommandCompiler();

  void compile(const char* inputString);
  // TODO: Bind?

  static std::vector<std::string> commandStrings;
  std::vector<std::stack<CommandFunction*>> commandFunctions;
private:
  CommandScanner commandScanner;
};

#endif /* _CommandCompiler_h */
