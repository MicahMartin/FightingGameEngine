#include "domain_language/Compiler.h"


Compiler::Compiler(){}
Compiler::~Compiler(){}

void Compiler::parsePrecedence(Precedence precedence){
  advance();
  // printf("looking for prefix rule for token %s \n", std::string(parser.previous.start, parser.previous.length).c_str());
  ParseFn prefixRule = getRule(parser.previous.type)->prefix;
  if (prefixRule == NULL) {
    error("Prefix rule is null");
    return;
  }
  bool canAssign = precedence <= PREC_ASSIGNMENT;
  // prefix rule should be Compiler::number
  (*this.*prefixRule)(canAssign);
  // printf("called some prefixRule\n");

  while (precedence <= getRule(parser.current.type)->precedence) {
    // printf("we in here\n");
    advance();
    ParseFn infixRule = getRule(parser.previous.type)->infix;
    // theres gotta be a better way to write this lol
    (*this.*infixRule)(canAssign);
  }

  if (canAssign && match(TOKEN_EQUAL)) {
    error("Invalid assignment target.");
    expression();
  }
}

ParseRule* Compiler::getRule(TokenType type) {
  return &rules[type];
}

uint8_t Compiler::makeConstant(Value value) {
  uint8_t symbolIndex = scriptPointer->writeSymbol(value);
  return symbolIndex;
}

uint8_t Compiler::identifierConstant(Token* name) {
  std::string* stringVal = new std::string(name->start, name->length);
  Value val = STRING_VAL(stringVal);
  return makeConstant(val);
}

int Compiler::resolveLocal(Token* name) {
  for (int i = scriptPointer->localCount - 1; i >= 0; i--) {   
    Local* local = &scriptPointer->locals[i];                  
    if (identifiersEqual(name, &local->name)) {           
      if (local->depth == -1) {
        error("Cannot read local variable in its own initializer.");
      }
      return i;                                           
    }                                                     
  }
  return -1;                                              
}

bool Compiler::identifiersEqual(Token* a, Token* b) {
  if (a->length != b->length) return false;
  return memcmp(a->start, b->start, a->length) == 0;
}

void Compiler::addLocal(Token name) {
  if (scriptPointer->localCount == 256) {
    error("Too many local variables in function.");
    return;
  }
  Local* local = &scriptPointer->locals[scriptPointer->localCount++];
  local->name = name;
  local->depth = -1;
  local->depth = scriptPointer->scopeDepth;
}

void Compiler::emitConstant(Value value) {
  // printf("in emitConstant\n");
  uint8_t symbolIndex = makeConstant(value);
  emitBytes(OP_CONSTANT, symbolIndex);
}

void Compiler::patchJump(int offset){
  // -2 to adjust for the bytecode for the jump offset itself.
  int jump = currentScript()->code.size() - offset - 2;

  if (jump > UINT16_MAX) {
    error("Too much code to jump over.");
  }

  currentScript()->code[offset] = (jump >> 8) & 0xff;
  currentScript()->code[offset + 1] = jump & 0xff;
}

int Compiler::emitJump(uint8_t offset) {
  emitByte(offset);
  emitByte(0xff);
  emitByte(0xff);
  return currentScript()->code.size()- 2;
}

void Compiler::engineCall(bool canAssign){
  // printf("what is the value of canAssign? %d\n", canAssign);
  std::string callString(parser.previous.start, parser.previous.length);
  // printf("in engine call! %s\n", callString.c_str());
  emitByte(engineCallMap.at(callString));
}

void Compiler::engineCallArg(bool canAssign){
  std::string callString(parser.previous.start, parser.previous.length);
  long value = strtol(parser.current.start, NULL, 10);
  // printf("in parameterized engine call! %s(%ld)\n", callString.c_str(), value);

  consume(TOKEN_NUMBER, "expected number after parameterized engine call");
  uint8_t symbolIndex = makeConstant(NUMBER_VAL(value));
  emitBytes(OP_CONSTANT, symbolIndex);
  emitByte(engineCallMap.at(callString));
}

void Compiler::number(bool canAssign) {
  // printf("in number\n");
  long value = strtol(parser.previous.start, NULL, 10);
  // printf("working with val %ld \n", value);
  emitConstant(NUMBER_VAL(value));
}

void Compiler::string(bool canAssign) {
  // printf("in string\n");
  // TODO: keep a refernce to this somehow
  std::string* stringVal = new std::string(parser.previous.start+1, parser.previous.length-2);
  // printf("creating string %s, the address %p\n", stringVal->c_str(), stringVal);
  Value val = STRING_VAL(stringVal);
  // printf("creating string on some weird shit?? %s, the address %p\n", val.as.string->c_str(), val.as.string);
  emitConstant(val);
}

void Compiler::namedVariable(Token name, bool canAssign) {
  uint8_t getOp, setOp;
  int arg = resolveLocal(&name);
  if (arg != -1) {
    getOp = OP_GET_LOCAL;
    setOp = OP_SET_LOCAL;
  } else {
    arg = identifierConstant(&name);
    getOp = OP_GET_GLOBAL;
    setOp = OP_SET_GLOBAL;
  }

  if (canAssign && match(TOKEN_EQUAL)) {
    expression();
    emitBytes(setOp, (uint8_t)arg);
  } else {
    emitBytes(getOp, (uint8_t)arg);
  }
}

void Compiler::variable(bool canAssign) {
  // printf("in variable\n");
  namedVariable(parser.previous, canAssign);
}

void Compiler::unary(bool canAssign) {
  // printf("in unary\n");
  TokenType operatorType = parser.previous.type;

  // Compile the operand.
  parsePrecedence(PREC_UNARY);     

  // Emit the operator instruction.
  switch (operatorType) {
    case TOKEN_BANG: emitByte(OP_NOT); break;
    case TOKEN_MINUS: emitByte(OP_NEGATE); break;
    default:
      return; // Unreachable.
  }
}

void Compiler::binary(bool canAssign) {
  // Remember the operator.
  // printf("in binary\n");
  TokenType operatorType = parser.previous.type;

  // Compile the right operand.
  ParseRule* rule = getRule(operatorType);
  parsePrecedence((Precedence)(rule->precedence + 1));

  // Emit the operator instruction.
  switch (operatorType) {
    case TOKEN_BANG_EQUAL:    emitBytes(OP_EQUAL, OP_NOT); break;
    case TOKEN_EQUAL_EQUAL:   emitByte(OP_EQUAL); break;
    case TOKEN_GREATER:       emitByte(OP_GREATER); break;
    case TOKEN_GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT); break;
    case TOKEN_LESS:          emitByte(OP_LESS); break;
    case TOKEN_LESS_EQUAL:    emitBytes(OP_GREATER, OP_NOT); break;
    case TOKEN_PLUS:          emitByte(OP_ADD); break;
    case TOKEN_MINUS:         emitByte(OP_SUBTRACT); break;
    case TOKEN_STAR:          emitByte(OP_MULTIPLY); break;
    case TOKEN_SLASH:         emitByte(OP_DIVIDE); break;
    default:
      return; // Unreachable.
  }
}

void Compiler::literal(bool canAssign) {
  switch (parser.previous.type) {
    case TOKEN_FALSE: emitByte(OP_FALSE); break;
    case TOKEN_NIL: emitByte(OP_NIL); break;
    case TOKEN_TRUE: emitByte(OP_TRUE); break;
    default:
      return; // Unreachable.
  }
}

void Compiler::grouping(bool canAssign) {
  expression();
  consume(TOKEN_RIGHT_PAREN, "you're missing a ')' after the expression dingus");
}

void Compiler::logicalOr(bool canAssign){
  int elseJump = emitJump(OP_JUMP_IF_FALSE);
  int endJump = emitJump(OP_JUMP);

  patchJump(elseJump);
  emitByte(OP_POP);

  parsePrecedence(PREC_OR);
  patchJump(endJump);
}

void Compiler::logicalAnd(bool canAssign){
  int endJump = emitJump(OP_JUMP_IF_FALSE);

  emitByte(OP_POP);
  parsePrecedence(PREC_AND);

  patchJump(endJump);
}

bool Compiler::check(TokenType expected) {
  return parser.current.type == expected;
}

bool Compiler::match(TokenType expected) {
  if(!check(expected)) {
    return false;
  }
  advance();
  return true;
}

void Compiler::advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanner.scan();
    // printf("done scan, parser at %s\n", std::string(parser.current.start, parser.current.length).c_str());
    if (parser.current.type != TOKEN_ERROR) break;

    errorAtCurrent(parser.current.start);
  }
}

void Compiler::expression() {
  // printf("in expression\n");
  parsePrecedence(PREC_ASSIGNMENT);
}

void Compiler::expressionStatement() {
  expression();
  consume(TOKEN_SEMICOLON, "Expect ';' after expression.");
  emitByte(OP_POP);
}

void Compiler::beginScope(){
  scriptPointer->scopeDepth++;
}

void Compiler::endScope(){
  scriptPointer->scopeDepth--;
  while (scriptPointer->localCount > 0 && scriptPointer->locals[scriptPointer->localCount - 1].depth > scriptPointer->scopeDepth) {
    emitByte(OP_POP);
    scriptPointer->localCount--;
  }
}

void Compiler::block(){
  while (!check(TOKEN_RIGHT_BRACE) && !check(TOKEN_EOF)) {
    // printf("in block\n");
    declaration();
  }

  consume(TOKEN_RIGHT_BRACE, "Expect '}' after block.");
}

void Compiler::whileStatement() {
  int loopStart = currentScript()->code.size();

  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'while'.");
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

  int exitJump = emitJump(OP_JUMP_IF_FALSE);

  emitByte(OP_POP);
  statement();
  emitLoop(loopStart);


  patchJump(exitJump);
  emitByte(OP_POP);
}

void Compiler::ifStatement() {
  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expect ')' after condition.");

  int thenJump = emitJump(OP_JUMP_IF_FALSE);
  emitByte(OP_POP);
  statement();
  int elseJump = emitJump(OP_JUMP);


  patchJump(thenJump);
  emitByte(OP_POP);
  if (match(TOKEN_ELSE)) statement();
  patchJump(elseJump);
  // printf("if parsed successfully\n");
}

void Compiler::forStatement() {
  beginScope();

  consume(TOKEN_LEFT_PAREN, "Expect '(' after 'for'.");
  if (match(TOKEN_SEMICOLON)) {
    // No initializer.
  } else if (match(TOKEN_VAR)) {
    varDeclaration();
    // printf("var declared\n");
  } else {
    expressionStatement();
  }

  int loopStart = scriptPointer->code.size();
  int exitJump = -1;
  if (!match(TOKEN_SEMICOLON)) {
    expression();
    consume(TOKEN_SEMICOLON, "Expect ';' after loop condition.");

    // Jump out of the loop if the condition is false.
    exitJump = emitJump(OP_JUMP_IF_FALSE);
    emitByte(OP_POP); // Condition.
  }

  if (!match(TOKEN_RIGHT_PAREN)) {
    int bodyJump = emitJump(OP_JUMP);

    int incrementStart = currentScript()->code.size();
    expression();
    emitByte(OP_POP);
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after for clauses.");

    emitLoop(loopStart);
    loopStart = incrementStart;
    patchJump(bodyJump);
  }

  statement();
  emitLoop(loopStart);

  if (exitJump != -1) {
    patchJump(exitJump);
    emitByte(OP_POP); // Condition.
  }
  endScope();
}

void Compiler::statement() {
  // Godlike architecture decisions
  // TODO: please dont forget to refactor this shit show
  if(match(TOKEN_PRINT)){
    printStatement();
  } else if (match(TOKEN_GET_ANIM_TIME)) {
    engineCallStatement(OP_GET_ANIM_TIME);
  } else if (match(TOKEN_GET_HIT_STUN)) {
    engineCallStatement(OP_GET_HIT_STUN);
  } else if (match(TOKEN_GET_STATE_TIME)) {
    // printf("matching get_state_time statement\n");
    engineCallStatement(OP_GET_STATE_TIME);
  } else if (match(TOKEN_GET_Y_POS)) {
    engineCallStatement(OP_GET_Y_POS);
  } else if (match(TOKEN_GET_INPUT)) {
    engineCallExpressionStatement(OP_GET_INPUT);
  } else if (match(TOKEN_GET_STATE_NUM)) {
    engineCallStatement(OP_GET_STATE_NUM);
  } else if (match(TOKEN_GET_CONTROL)) {
    engineCallStatement(OP_GET_CONTROL);
  } else if (match(TOKEN_WAS_PRESSED)) {
    // printf("matching wasPressed\n");
    engineCallExpressionStatement(OP_WAS_PRESSED);
  } else if (match(TOKEN_GET_COMBO)) {
    engineCallStatement(OP_GET_COMBO);
  } else if (match(TOKEN_HAS_AIR_ACTION)) {
    engineCallStatement(OP_HAS_AIR_ACTION);
  } else if (match(TOKEN_CHANGE_STATE)) {
    // printf("changing state\n");
    engineCallExpressionStatement(OP_CHANGE_STATE);
  } else if (match(TOKEN_CANCEL_STATE)) {
    engineCallExpressionStatement(OP_CANCEL_STATE);
  } else if (match(TOKEN_VELSET_X)) {
    engineCallExpressionStatement(OP_VELSET_X);
  } else if (match(TOKEN_NEG_VELSET_X)) {
    engineCallExpressionStatement(OP_NEG_VELSET_X);
  } else if (match(TOKEN_VELSET_Y)) {
    engineCallExpressionStatement(OP_VELSET_Y);
  } else if (match(TOKEN_MOVE_U)) {
    engineCallExpressionStatement(OP_MOVE_U);
  } else if (match(TOKEN_MOVE_D)) {
    engineCallExpressionStatement(OP_MOVE_D);
  } else if (match(TOKEN_MOVE_F)) {
    engineCallExpressionStatement(OP_MOVE_F);
  } else if (match(TOKEN_MOVE_B)) {
    engineCallExpressionStatement(OP_MOVE_B);
  } else if (match(TOKEN_SET_CONTROL)) {
    engineCallExpressionStatement(OP_SET_CONTROL);
  } else if (match(TOKEN_SET_COMBO)) {
    engineCallExpressionStatement(OP_SET_COMBO);
  } else if (match(TOKEN_SET_GRAVITY)) {
    engineCallExpressionStatement(OP_SET_GRAVITY);
  } else if (match(TOKEN_SET_NOGRAV_COUNT)) {
    engineCallExpressionStatement(OP_SET_NOGRAV_COUNT);
  } else if (match(TOKEN_SET_AIR_ACTION)) {
    engineCallExpressionStatement(OP_SET_AIR_ACTION);
  } else if (match(TOKEN_RESET_ANIM)) {
    engineCallStatement(OP_RESET_ANIM);
  } else if (match(TOKEN_CHECK_COMMAND)) {
    engineCallExpressionStatement(OP_CHECK_COMMAND);
  } else if (match(TOKEN_FOR)) {
    forStatement();
  } else if (match(TOKEN_WHILE)) {     
    whileStatement();
  }
  else if (match(TOKEN_IF)) {        
    ifStatement(); 
  }
  else if (match(TOKEN_LEFT_BRACE)) {
    beginScope();                      
    block();                           
    endScope(); 
  }
  else {
    // printf("nothing matched\n");
    expressionStatement();
  }
}

void Compiler::declareVariable() {
  // Global variables are implicitly declared.
  if (scriptPointer->scopeDepth == 0) return;

  // printf("adding local\n");
  Token* name = &parser.previous;
  for (int i = scriptPointer->localCount - 1; i >= 0; i--) {
    Local* local = &scriptPointer->locals[i];
    if (local->depth != -1 && local->depth < scriptPointer->scopeDepth) {
      break;
    }

    if (identifiersEqual(name, &local->name)) {
      error("Variable with this name already declared in this scope.");
    }
  }
  addLocal(*name);
  // printf("added local\n");
}

uint8_t Compiler::parseVariable(const char* errorMessage) {
  // printf("in parseVariable\n");
  consume(TOKEN_IDENTIFIER, errorMessage);              
  declareVariable();
  if (scriptPointer->scopeDepth > 0) return 0;
  return identifierConstant(&parser.previous);
}

void Compiler::markInitialized() {
  scriptPointer->locals[scriptPointer->localCount - 1].depth = scriptPointer->scopeDepth;
}

void Compiler::defineVariable(uint8_t var) {
  if (scriptPointer->scopeDepth > 0) {
    markInitialized();
    return;
  }
  // printf("defining global\n");
  emitBytes(OP_DEFINE_GLOBAL, var);
}

void Compiler::varDeclaration() {
  // printf("in var decl\n");
  uint8_t global = parseVariable("Expect variable name.");
  // printf("var parsed, parser currently at %s\n", std::string(parser.current.start, parser.current.length).c_str());

  if (match(TOKEN_EQUAL)) {
    // printf("matched equal\n");
    expression();
  } else {
    // printf("emmiting null byte\n");
    emitByte(OP_NIL);
  }
  // printf("going to consume until semicolon\n");
  consume(TOKEN_SEMICOLON, "Expect ';' after variable declaration.");

  // printf("about to define\n");
  defineVariable(global);
}

void Compiler::declaration() {
  if (match(TOKEN_VAR)) {
    // printf("we are declaring a var!\n");
    varDeclaration();
    // printf("done var declaration!\n");
  } else {
    // printf("we are declaring a statement!\n");
    statement();
  }
  if (parser.panicMode) synchronize();
}


void Compiler::consume(TokenType type, const char* syntaxErrorMessage){
  if (parser.current.type == type) {
    advance();
    return;
  }

  errorAtCurrent(syntaxErrorMessage);
}

bool Compiler::compile(const char *source, Script* script, const char* scriptTag){
  scanner.initScanner(source);
  scriptPointer = script;

  parser.hadError = false;
  parser.panicMode = false;

  // printf("running first advance\n");
  advance();
  // printf("first advance done\n");
  while (!match(TOKEN_EOF)) {
    // printf("not EOF\n");
    declaration();
  }
  // printf("matched EOF\n");
  emitByte(OP_RETURN);
  return !parser.hadError;
}


void Compiler::error(const char* message) {
  errorAt(&parser.previous, message);
}

void Compiler::errorAtCurrent(const char* message){
  errorAt(&parser.current, message);
}


void Compiler::errorAt(Token* token, const char* message){
  if (parser.panicMode) return;
  parser.panicMode = true;

  fprintf(stderr, "[line %d] Error", token->line);

  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {
    // Nothing.
  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->start);
  }

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

void Compiler::synchronize(){
  parser.panicMode = false;

  while (parser.current.type != TOKEN_EOF) {
    if (parser.previous.type == TOKEN_SEMICOLON) return;

    switch (parser.current.type) {
      case TOKEN_FUNC:
      case TOKEN_VAR:
      case TOKEN_FOR:
      case TOKEN_IF:
      case TOKEN_WHILE:
      case TOKEN_PRINT:
      case TOKEN_RETURN:
      return;

      default: ; //nothing
    }

    advance();
  }
}

Script* Compiler::currentScript(){
  return scriptPointer;
}

void Compiler::printStatement(){
  expression();
  consume(TOKEN_SEMICOLON, "expected ; after value");
  emitByte(OP_PRINT);
}

void Compiler::engineCallExpressionStatement(OpCode call){
  expression();
  consume(TOKEN_SEMICOLON, "expected ; after value");
  emitByte(call);
}

void Compiler::engineCallStatement(OpCode call){
  consume(TOKEN_SEMICOLON, "expected ; after value");
  emitByte(call);
}

void Compiler::emitByte(uint8_t byte) {
  scriptPointer->writeByte(byte, parser.previous.line);
}

void Compiler::emitBytes(uint8_t firstByte, uint8_t secondByte) {
  emitByte(firstByte);
  emitByte(secondByte);
}

void Compiler::emitLoop(int loopStart) {
  emitByte(OP_LOOP);

  int offset = currentScript()->code.size() - loopStart + 2;
  if (offset > UINT16_MAX) error("Loop body too large.");

  emitByte((offset >> 8) & 0xff);
  emitByte(offset & 0xff);
}

