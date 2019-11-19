#include "input/CommandScanner.h"

CommandScanner::CommandScanner(){};
CommandScanner::~CommandScanner(){};

std::vector<CommandToken> CommandScanner::scan(const char* inputString) {
  scannerStart = inputString;
  scannerCurrent = scannerStart;
  std::vector<CommandToken> returnVect;

  while(!isAtEnd()){
    skipWhitespace();
    scannerStart = scannerCurrent;
    char c = advance();

    if (isAlpha(c)){
      while(isAlpha(peek())){
        advance();
      }
      returnVect.push_back(makeToken(getInputType()));
      scannerStart = scannerCurrent;
    };

    switch (c) {
      case '~': 
        returnVect.push_back(makeToken(CTOKEN_RELEASED)); 
        scannerStart = scannerCurrent;
        break;
      case '*': 
        returnVect.push_back(makeToken(CTOKEN_HELD));
        scannerStart = scannerCurrent;
        break;
      case '@': 
        returnVect.push_back(makeToken(CTOKEN_ANY));
        scannerStart = scannerCurrent;
        break;
      case '!': 
        returnVect.push_back(makeToken(CTOKEN_NOT));
        scannerStart = scannerCurrent;
        break;
      case '&': 
        returnVect.push_back(makeToken(CTOKEN_AND));
        scannerStart = scannerCurrent;
        break;
      case '|': 
        returnVect.push_back(makeToken(CTOKEN_OR));
        scannerStart = scannerCurrent;
        break;
      case ',':
        returnVect.push_back(makeToken(CTOKEN_DELIM));
        scannerStart = scannerCurrent;
        break;
    }
  }
  if(isAtEnd()){
    returnVect.push_back(makeToken(CTOKEN_END));
  }

  return returnVect;
}

CommandTokenType CommandScanner::getInputType() {
  switch (scannerStart[0]) {
    case 'N': return CTOKEN_NEUTRAL;
    case 'F': return CTOKEN_FORWARD;
    case 'B': return CTOKEN_BACK;
    case 'U':
      if (scannerCurrent - scannerStart > 1) {
        switch (scannerStart[1]) {
          case 'F': return CTOKEN_UPFORWARD;
          case 'B': return CTOKEN_UPBACK;
        }
      }
      return CTOKEN_UP;
    case 'D':
      if (scannerCurrent - scannerStart > 1) {
        switch (scannerStart[1]) {
          case 'F': return CTOKEN_DOWNFORWARD;
          case 'B': return CTOKEN_DOWNBACK;
        }
      }
      return CTOKEN_DOWN;
    case 'L':
      if (scannerCurrent - scannerStart > 1) {
        switch (scannerStart[1]) {
          case 'P': return CTOKEN_LP;
          case 'K': return CTOKEN_LK;
        }
      }
    case 'M':
      if (scannerCurrent - scannerStart > 1) {
        switch (scannerStart[1]) {
          case 'P': return CTOKEN_MP;
          case 'K': return CTOKEN_MK;
        }
      }
  }
}

CommandToken CommandScanner::makeToken(CommandTokenType tokenType) {
  CommandToken token;                                          
  token.type = tokenType;                                    
  token.start = scannerStart;                          
  token.length = (int)(scannerCurrent - scannerStart);

  return token;
}

bool CommandScanner::isAtEnd() {
  return *scannerCurrent == '\0';
}

char CommandScanner::peek(){
  return *scannerCurrent;
}

char CommandScanner::peekNext(){
  if (isAtEnd()) return '\0';
  return *scannerCurrent+1;
}

char CommandScanner::advance(){
  scannerCurrent++;
  return scannerCurrent[-1];
}


bool CommandScanner::match(char expected){
  if (isAtEnd()) return false;
  if (*scannerCurrent!= expected) return false;

  scannerCurrent++;
  return true;
}


void CommandScanner::skipWhitespace(){
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      default:
        return;
    }
  }
}

bool CommandScanner::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
