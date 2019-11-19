#ifndef _CommandScanner_h
#define _CommandScanner_h

#include <vector>
typedef enum {
  CTOKEN_NEUTRAL, CTOKEN_FORWARD, CTOKEN_BACK, CTOKEN_UP, CTOKEN_DOWN,
  CTOKEN_UPFORWARD, CTOKEN_UPBACK, CTOKEN_DOWNFORWARD, CTOKEN_DOWNBACK,
  CTOKEN_LP, CTOKEN_LK, CTOKEN_MP, CTOKEN_MK,

  CTOKEN_RELEASED, CTOKEN_HELD,

  CTOKEN_AND, CTOKEN_OR,

  CTOKEN_ANY, 
  CTOKEN_NOT,
  CTOKEN_DELIM,
  CTOKEN_END,

} CommandTokenType;

typedef struct {
  CommandTokenType type;
  const char* start;
  uint8_t length;
} CommandToken;

class CommandScanner {
public:
  CommandScanner();
  ~CommandScanner();

  std::vector<CommandToken> scan(const char* inputString);

  CommandTokenType getInputType();
  CommandToken makeToken(CommandTokenType type);
  bool isAtEnd();
  char peek();
  char peekNext();
  char advance();
  bool match(char expected);
  void skipWhitespace();
  bool isAlpha(char c);
  CommandTokenType checkKeyword(int start, int end, const char* rest, CommandTokenType type);

private:
  const char* scannerStart;
  const char* scannerCurrent;
};

#endif 
