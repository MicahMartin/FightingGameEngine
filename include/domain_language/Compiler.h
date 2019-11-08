#ifndef _Compiler_h
#define _Compiler_h

#include "domain_language/Common.h"
#include "domain_language/Scanner.h"
#include "domain_language/Script.h"

typedef struct {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
} Parser;

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT,  // =
  PREC_OR,          // or
  PREC_AND,         // and
  PREC_EQUALITY,    // == !=
  PREC_COMPARISON,  // < > <= >=
  PREC_TERM,        // + -
  PREC_FACTOR,      // * /
  PREC_UNARY,       // ! -
  PREC_CALL,        // () []
  PREC_PRIMARY
} Precedence;

class Compiler;
typedef void (Compiler::*ParseFn)(bool canAssign);

typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRule;

class Compiler {
public:
  Compiler();
  ~Compiler();

  bool compile(const char* source, Script* script, const char* scriptTag);
  void emitByte(uint8_t byte);
  void emitBytes(uint8_t firstByte, uint8_t secondByte);
  Script* currentScript();

private:
  Scanner scanner;
  Parser parser;
  Script* scriptPointer;

  void printStatement();
  void engineCallExpressionStatement(OpCode call);
  void engineCallStatement(OpCode call);
  void parsePrecedence(Precedence precedence);
  ParseRule* getRule(TokenType type);


  uint8_t makeConstant(Value value);
  uint8_t identifierConstant(Token* name);
  int resolveLocal(Token* name);
  bool identifiersEqual(Token* a, Token* b);
  void addLocal(Token name);
  void emitConstant(Value value);
  void emitLoop(int loopStart);
  int emitJump(uint8_t offset);

  void number(bool canAssign);
  void string(bool canAssign);
  void variable(bool canAssign);
  void unary(bool canAssign);
  void binary(bool canAssign);
  void literal(bool canAssign);
  void engineCall(bool canAssign);
  void grouping(bool canAssign);
  void logicalOr(bool canAssign);
  void logicalAnd(bool canAssign);
  void namedVariable(Token name, bool canAssign);

  bool match(TokenType expected);
  bool check(TokenType expected);

  void patchJump(int jumpLength);
  void advance();
  void expressionStatement();
  void expression();
  void whileStatement();
  void forStatement();
  void ifStatement();
  void statement();
  void block();
  void beginScope();
  void endScope();
  uint8_t parseVariable(const char* syntaxErrorMessage);
  void defineVariable(uint8_t var);
  void declareVariable();
  void varDeclaration();
  void declaration();
  void consume(TokenType, const char* syntaxErrorMessage);

  void error(const char* message);
  void errorAtCurrent(const char* message);
  void errorAt(Token* token, const char* message);
  void synchronize();
  void markInitialized();

  ParseRule rules[63] = {
    { &Compiler::grouping, NULL,    PREC_NONE },                  // TOKEN_LEFT_PAREN
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_RIGHT_PAREN
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_LEFT_BRACE
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_RIGHT_BRACE
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_COMMA
    { &Compiler::unary,    &Compiler::binary,  PREC_TERM },       // TOKEN_MINUS
    { NULL,     &Compiler::binary,  PREC_TERM },                  // TOKEN_PLUS
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_SEMICOLON
    { NULL,     &Compiler::binary,  PREC_FACTOR },                // TOKEN_SLASH
    { NULL,     &Compiler::binary,  PREC_FACTOR },                // TOKEN_STAR
    { &Compiler::unary,     NULL,    PREC_NONE },                             // TOKEN_BANG
    { NULL,     &Compiler::binary,    PREC_EQUALITY},                             // TOKEN_BANG_EQUAL
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_EQUAL
    { NULL,     &Compiler::binary,    PREC_EQUALITY },                             // TOKEN_EQUAL_EQUAL
    { NULL,     &Compiler::binary,    PREC_COMPARISON },                             // TOKEN_GREATER
    { NULL,     &Compiler::binary,    PREC_COMPARISON },                             // TOKEN_GREATER_EQUAL
    { NULL,     &Compiler::binary,    PREC_COMPARISON },                             // TOKEN_LESS
    { NULL,     &Compiler::binary,    PREC_COMPARISON },                             // TOKEN_LESS_EQUAL
    { &Compiler::variable,     NULL,    PREC_NONE },                             // TOKEN_IDENTIFIER
    { &Compiler::string,     NULL,    PREC_NONE },                             // TOKEN_STRING
    { &Compiler::number,   NULL,    PREC_NONE },                  // TOKEN_NUMBER
    { NULL,     &Compiler::logicalAnd,    PREC_AND},                             // TOKEN_AND
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_ELSE
    { &Compiler::literal,     NULL,    PREC_NONE },                             // TOKEN_FALSE
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_FOR
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_FUN
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_IF
    { &Compiler::literal,     NULL,    PREC_NONE },                             // TOKEN_NIL
    { NULL,     &Compiler::logicalOr,    PREC_OR},                             // TOKEN_OR
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_PRINT
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_RETURN
    { &Compiler::literal,     NULL,    PREC_NONE },                             // TOKEN_TRUE
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_VAR
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_WHILE
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_ERROR
    { NULL,     NULL,    PREC_NONE },                             // TOKEN_EOF
    { &Compiler::engineCall, NULL, PREC_CALL },  // TOKEN_GET_ANIM_TIME, 
    { NULL, NULL, PREC_NONE },  // TOKEN_GET_HIT_STUN, 
    { &Compiler::engineCall, &Compiler::engineCall, PREC_CALL},  // TOKEN_GET_STATE_TIME,
    { NULL, NULL, PREC_NONE },  // TOKEN_GET_Y_POS, 
    { NULL, NULL, PREC_NONE },  // TOKEN_GET_INPUT, 
    { NULL, NULL, PREC_NONE },  // TOKEN_GET_STATE_NUM,
    { NULL, NULL, PREC_NONE },  // TOKEN_GET_COMBO, 
    { NULL, NULL, PREC_NONE },  // TOKEN_GET_CONTROL,

    { NULL, NULL, PREC_NONE },  // TOKEN_CHECK_COMMAND, 
    { NULL, NULL, PREC_NONE },  // TOKEN_WAS_PRESSED, 
    { NULL, NULL, PREC_NONE },  // TOKEN_HAS_AIR_ACTION,

    { NULL, NULL, PREC_NONE },  // TOKEN_CHANGE_STATE, 
    { NULL, NULL, PREC_NONE },  // TOKEN_CANCEL_STATE,

    { NULL, NULL, PREC_NONE },  // TOKEN_VELSET_X, 
    { NULL, NULL, PREC_NONE },  // TOKEN_NEG_VELSET_X, 
    { NULL, NULL, PREC_NONE },  // TOKEN_VELSET_Y,
    { NULL, NULL, PREC_NONE },  // TOKEN_MOVE_F, 
    { NULL, NULL, PREC_NONE },  // TOKEN_MOVE_B, 
    { NULL, NULL, PREC_NONE },  // TOKEN_MOVE_U, 
    { NULL, NULL, PREC_NONE },  // TOKEN_MOVE_D,
    { NULL, NULL, PREC_NONE },  // TOKEN_SET_CONTROL, 
    { NULL, NULL, PREC_NONE },  // TOKEN_SET_COMBO, 
    { NULL, NULL, PREC_NONE },  // TOKEN_SET_GRAVITY,
    { NULL, NULL, PREC_NONE },  // TOKEN_SET_NOGRAV_COUNT, 
    { NULL, NULL, PREC_NONE },  // TOKEN_SET_AIR_ACTION, 
    { NULL, NULL, PREC_NONE },  // TOKEN_RESET_ANIM,
  };
};

#endif
