#ifndef _Compiler_h
#define _Compiler_h

#include "domain_language/Common.h"
#include "domain_language/Scanner.h"
#include "domain_language/Script.h"

struct Parser {
  Token current;
  Token previous;
  bool hadError;
  bool panicMode;
};

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

struct ParseRule {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
};

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
  void engineCallArg(bool canAssign);
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

  ParseRule groupRule = { &Compiler::grouping, NULL, PREC_NONE };
  ParseRule noRule = { NULL, NULL, PREC_NONE };
  ParseRule negativeThenMinusRule = { &Compiler::unary, &Compiler::binary, PREC_TERM };
  ParseRule multiplyDivideRule = { NULL, &Compiler::binary,  PREC_FACTOR };
  ParseRule binaryTermRule = { NULL, &Compiler::binary, PREC_TERM };
  ParseRule engineRule = { &Compiler::engineCall, &Compiler::engineCall, PREC_NONE };
  ParseRule unaryRule = { &Compiler::unary, NULL, PREC_NONE };
  ParseRule binaryEqualityRule = { NULL, &Compiler::binary, PREC_EQUALITY };
  ParseRule binaryComparisonRule = { NULL, &Compiler::binary, PREC_COMPARISON };
  ParseRule variableRule = { &Compiler::variable, NULL, PREC_NONE };
  ParseRule stringRule = { &Compiler::string, NULL, PREC_NONE };
  ParseRule numberLiteralRule = { &Compiler::number, NULL, PREC_NONE };
  ParseRule logicalAndRule = { NULL, &Compiler::logicalAnd, PREC_AND };
  ParseRule valueLiteralRule = { &Compiler::literal, NULL, PREC_NONE };
  ParseRule logicalOrRule = { NULL, &Compiler::logicalOr, PREC_OR };
  ParseRule engineCallRule = { &Compiler::engineCall, &Compiler::engineCall, PREC_NONE };
  ParseRule engineCallArgRule = { &Compiler::engineCallArg, &Compiler::engineCallArg, PREC_NONE };

  std::unordered_map<TokenType, ParseRule> rules = {
    {TOKEN_RIGHT_PAREN, noRule},
    {TOKEN_LEFT_BRACE, noRule},
    {TOKEN_RIGHT_BRACE, noRule},
    {TOKEN_COMMA, noRule},
    {TOKEN_SEMICOLON, noRule},
    {TOKEN_EQUAL, noRule},
    {TOKEN_IF, noRule},
    {TOKEN_NIL, noRule},
    {TOKEN_PRINT, noRule},
    {TOKEN_RETURN, noRule},
    {TOKEN_VAR, noRule},
    {TOKEN_ERROR, noRule},
    {TOKEN_EOF, noRule},
    {TOKEN_RESET_ANIM, noRule},
    {TOKEN_LEFT_PAREN, groupRule},
    {TOKEN_MINUS, negativeThenMinusRule},
    {TOKEN_PLUS, binaryTermRule},
    {TOKEN_SLASH, multiplyDivideRule},
    {TOKEN_STAR, multiplyDivideRule},
    {TOKEN_BANG, unaryRule},
    {TOKEN_BANG_EQUAL, binaryEqualityRule},
    {TOKEN_EQUAL_EQUAL, binaryEqualityRule},
    {TOKEN_GREATER, binaryComparisonRule},
    {TOKEN_GREATER_EQUAL, binaryComparisonRule},
    {TOKEN_LESS, binaryComparisonRule},
    {TOKEN_LESS_EQUAL, binaryComparisonRule},
    {TOKEN_IDENTIFIER, variableRule},
    {TOKEN_STRING, stringRule},
    {TOKEN_NUMBER, numberLiteralRule},
    {TOKEN_FALSE, valueLiteralRule},
    {TOKEN_TRUE, valueLiteralRule},
    {TOKEN_AND, logicalAndRule},
    {TOKEN_OR, logicalOrRule},
    {TOKEN_GET_ANIM_TIME, engineCallRule},
    {TOKEN_GET_HIT_STUN, engineCallRule},
    {TOKEN_GET_BLOCK_STUN, engineCallRule},
    {TOKEN_SET_BLOCK_STUN, engineCallArgRule},
    {TOKEN_GET_STATE_TIME, engineCallRule},
    {TOKEN_GET_Y_POS, engineCallRule},
    {TOKEN_GET_INPUT, engineCallArgRule},
    {TOKEN_GET_STATE_NUM, engineCallRule},
    {TOKEN_GET_COMBO, engineCallRule},
    {TOKEN_GET_CONTROL, engineCallRule},
    {TOKEN_GET_IS_ALIVE, engineCallRule},
    {TOKEN_CHECK_COMMAND, engineCallArgRule},
    {TOKEN_WAS_PRESSED, engineCallArgRule},
    {TOKEN_HAS_AIR_ACTION, engineCallRule},
    {TOKEN_CHANGE_STATE, engineCallArgRule},
    {TOKEN_CANCEL_STATE, engineCallArgRule},
    {TOKEN_VELSET_X, engineCallArgRule},
    {TOKEN_NEG_VELSET_X, engineCallArgRule},
    {TOKEN_VELSET_Y, engineCallArgRule},
    {TOKEN_MOVE_F, engineCallArgRule},
    {TOKEN_MOVE_B, engineCallArgRule},
    {TOKEN_MOVE_U, engineCallArgRule},
    {TOKEN_MOVE_D, engineCallArgRule},
    {TOKEN_SET_CONTROL, engineCallArgRule},
    {TOKEN_SET_COMBO, engineCallArgRule},
    {TOKEN_SET_GRAVITY, engineCallArgRule},
    {TOKEN_SET_NOGRAV_COUNT, engineCallRule},
    {TOKEN_SET_AIR_ACTION, engineCallRule},
    {TOKEN_SET_HIT_STUN, engineCallArgRule},
    {TOKEN_GET_ENTITY_STATUS, engineCallArgRule},
    {TOKEN_ACTIVATE_ENTITY, engineCallArgRule},
    {TOKEN_DEACTIVATE_ENTITY, engineCallArgRule},
    {TOKEN_SNAP_TO_OPPONENT, engineCallArgRule},
    {TOKEN_GET_HIT_CANCEL, engineCallRule},
    {TOKEN_GET_WHIFF_CANCEL, engineCallRule},
    {TOKEN_SET_HIT_CANCEL, engineCallArgRule},
    {TOKEN_SET_WHIFF_CANCEL, engineCallArgRule},
    {TOKEN_SET_COUNTER, engineCallArgRule},
    {TOKEN_GET_METER, engineCallArgRule},
    {TOKEN_ADD_METER, engineCallArgRule},
    {TOKEN_GET_INSTALL, engineCallRule},
    {TOKEN_GET_Y_VEL, engineCallRule},
    {TOKEN_GET_X_VEL, engineCallRule},
    {TOKEN_SET_INSTALL, engineCallArgRule},
    {TOKEN_SUBTRACT_METER, engineCallArgRule},
  };
};

#endif
