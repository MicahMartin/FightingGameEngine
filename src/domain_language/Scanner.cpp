#include "domain_language/Common.h"
#include "domain_language/Scanner.h"
#include "string.h"


Scanner::Scanner(){}
Scanner::~Scanner(){}

void Scanner::initScanner(const char* source){
  start = source;
  current = source;
  line = 1;
}

bool Scanner::isAtEnd() {
  return *current == '\0';
}

bool Scanner::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
}

bool Scanner::isDigit(char c) {
  return c >= '0' && c <= '9';
}

Token Scanner::makeToken(TokenType tokenType) {
  Token token;                                          
  token.type = tokenType;                                    
  token.start = start;                          
  token.length = (int)(current - start);
  token.line = line;

  // printf("made token %s\n", std::string(start, current-start).c_str());
  return token;
}

Token Scanner::errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = line;

  return token;
}

char Scanner::advance(){
  current++;
  return current[-1];
}

char Scanner::peek(){
  return *current;
}

char Scanner::peekNext(){
  if (isAtEnd()) return '\0';
  return *current+1;
}

bool Scanner::match(char expected){
  if (isAtEnd()) return false;
  if (*current != expected) return false;

  current++;
  return true;
}

TokenType Scanner::checkKeyword(int begin, int length, const char* rest, TokenType type){
  if (current - start == begin  + length && memcmp(start + begin, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}


void Scanner::skipWhitespace(){
  for (;;) {
    char c = peek();
    switch (c) {
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;
      case '\n':
        line++;
        advance();
        break;
      case '/':
        if (peekNext() == '/') {
          // A comment goes until the end of the line.
          while (peek() != '\n' && !isAtEnd()) advance();
        } else {
          return;
        }
        break;
      default:
        return;
    }
  }
}

Token Scanner::string(){
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') line++;
    advance();
  }

  if (isAtEnd()) return errorToken("Unterminated string.");

  // The closing quote.
  advance();
  return makeToken(TOKEN_STRING);
}

Token Scanner::number(){
  while (isDigit(peek())) advance();
  return makeToken(TOKEN_NUMBER);
}

TokenType Scanner::identifierType() {
  switch (start[0]) {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
    case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);
    case 't': return checkKeyword(1, 3, "rue", TOKEN_TRUE);
    case 'f':
      if (current - start > 1) {
        switch (start[1]) {
          case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
          case 'u': return checkKeyword(2, 2, "nc", TOKEN_FUNC);
        }
      }
      break;
    case '$':
      std::string theStr(start, current - start);
      // printf("the str in question %s\n", theStr.c_str());
      auto type = keywordTypes.find(theStr);
      if (type == keywordTypes.end()) {
        // printf("couldnt find %s\n", theStr.c_str());
        return TOKEN_IDENTIFIER;
      } else {
        printf("found the %s\n", theStr.c_str());
        return type->second;
      }
      // return (type == keywordTypes.end() ? TOKEN_IDENTIFIER : type->second);
      break;
  }
  return TOKEN_IDENTIFIER;
}

Token Scanner::identifier(){
  // printf("in identifier\n");
  // consume all alphaNumeric chars for this identifier
  while (isAlpha(peek()) || isDigit(peek())) advance();
  return makeToken(identifierType());
}

Token Scanner::scan(){
  // printf("in scan\n");
  skipWhitespace();
  start = current;

  if (isAtEnd()) {
    // printf("scan reached end of file\n");
    return makeToken(TOKEN_EOF);
  }

  char c = advance();
  // printf("char from advance is %c\n", c);

  if (isAlpha(c)) return identifier();
  if (isDigit(c)) return number();
  

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS); 
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);
    case '!':
      return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=':
      return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<':
      return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>':
      return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
    case '"': return string();
  }

  return errorToken("Unexpected character.");
}
