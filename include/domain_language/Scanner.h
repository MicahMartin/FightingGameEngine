#ifndef _Scanner_h
#define _Scanner_h

#include <unordered_map>
#include <string>

typedef enum {
  // Single-character tokens.
  TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
  TOKEN_COMMA, TOKEN_MINUS, TOKEN_PLUS,
  TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

  // One or two character tokens.
  TOKEN_BANG, TOKEN_BANG_EQUAL,
  TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
  TOKEN_GREATER, TOKEN_GREATER_EQUAL,
  TOKEN_LESS, TOKEN_LESS_EQUAL,

  // Literals.
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,

  // Keywords.
  TOKEN_AND, TOKEN_ELSE, TOKEN_FALSE,
  TOKEN_FOR, TOKEN_FUNC, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
  TOKEN_PRINT, TOKEN_RETURN,
  TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,

  TOKEN_ERROR,
  TOKEN_EOF,

  // engine functions
  TOKEN_GET_ANIM_TIME, TOKEN_GET_HIT_STUN, TOKEN_GET_BLOCK_STUN, TOKEN_GET_STATE_TIME,
  TOKEN_GET_Y_POS, TOKEN_GET_INPUT, TOKEN_GET_STATE_NUM,
  TOKEN_GET_COMBO, TOKEN_GET_CONTROL, TOKEN_GET_IS_ALIVE,
  TOKEN_GET_HIT_CANCEL, TOKEN_GET_WHIFF_CANCEL, TOKEN_GET_METER,


  TOKEN_CHECK_COMMAND, TOKEN_WAS_PRESSED, TOKEN_HAS_AIR_ACTION,

  TOKEN_CHANGE_STATE, TOKEN_CANCEL_STATE,

  TOKEN_VELSET_X, TOKEN_NEG_VELSET_X, TOKEN_VELSET_Y,
  TOKEN_MOVE_F, TOKEN_MOVE_B, TOKEN_MOVE_U, TOKEN_MOVE_D,
  TOKEN_ADD_METER, TOKEN_SUBTRACT_METER,

  TOKEN_SET_CONTROL, TOKEN_SET_COMBO, TOKEN_SET_GRAVITY,
  TOKEN_SET_NOGRAV_COUNT, TOKEN_SET_AIR_ACTION, TOKEN_SET_HIT_STUN,
  TOKEN_SET_HIT_CANCEL, TOKEN_SET_WHIFF_CANCEL, TOKEN_SET_COUNTER,
  TOKEN_RESET_ANIM,

  TOKEN_ACTIVATE_ENTITY, TOKEN_DEACTIVATE_ENTITY,
  TOKEN_SNAP_TO_OPPONENT 

} TokenType;

struct Token {
  TokenType type;
  const char* start;
  int length;
  int line;
};

static const std::unordered_map<std::string, TokenType> keywordTypes {
  { "and", TOKEN_AND},
  { "else", TOKEN_ELSE },
  { "false", TOKEN_FALSE },
  { "func",  TOKEN_FUNC },
  { "for", TOKEN_FOR },
  { "if", TOKEN_IF },
  { "nil", TOKEN_NIL },
  { "or", TOKEN_OR },
  { "print", TOKEN_PRINT },
  { "return", TOKEN_RETURN },
  { "true", TOKEN_TRUE },
  { "var", TOKEN_VAR },
  { "while", TOKEN_WHILE },
  { "$get_anim_time",  TOKEN_GET_ANIM_TIME},
  { "$get_hit_stun",  TOKEN_GET_HIT_STUN},
  { "$get_state_time",  TOKEN_GET_STATE_TIME},
  { "$get_y_pos",  TOKEN_GET_Y_POS},
  { "$get_input",  TOKEN_GET_INPUT},
  { "$get_state_num",  TOKEN_GET_STATE_NUM},
  { "$get_control",  TOKEN_GET_CONTROL},
  { "$get_is_alive",  TOKEN_GET_IS_ALIVE},
  { "$was_pressed",  TOKEN_WAS_PRESSED},
  { "$get_combo",  TOKEN_GET_COMBO},
  { "$has_air_action",  TOKEN_HAS_AIR_ACTION},
  { "$change_state",  TOKEN_CHANGE_STATE},
  { "$cancel_state",  TOKEN_CANCEL_STATE},
  { "$velset_x",  TOKEN_VELSET_X},
  { "$neg_velset_x",  TOKEN_NEG_VELSET_X},
  { "$velset_y",  TOKEN_VELSET_Y},
  { "$move_u",  TOKEN_MOVE_U},
  { "$move_d",  TOKEN_MOVE_D},
  { "$move_f",  TOKEN_MOVE_F},
  { "$move_b",  TOKEN_MOVE_B},
  { "$set_control",  TOKEN_SET_CONTROL},
  { "$set_combo",  TOKEN_SET_COMBO},
  { "$set_gravity",  TOKEN_SET_GRAVITY},
  { "$set_nograv_count",  TOKEN_SET_NOGRAV_COUNT},
  { "$set_air_action",  TOKEN_SET_AIR_ACTION},
  { "$set_hit_stun",  TOKEN_SET_HIT_STUN},
  { "$reset_anim",  TOKEN_RESET_ANIM},
  { "$activate_entity",  TOKEN_ACTIVATE_ENTITY},
  { "$deactivate_entity",  TOKEN_DEACTIVATE_ENTITY},
  { "$snap_to_opponent",  TOKEN_SNAP_TO_OPPONENT},
  { "$check_command",  TOKEN_CHECK_COMMAND},
  { "$get_block_stun",  TOKEN_GET_BLOCK_STUN},
  { "$get_hit_cancel",  TOKEN_GET_HIT_CANCEL},
  { "$get_whiff_cancel",  TOKEN_GET_WHIFF_CANCEL},
  { "$set_hit_cancel",  TOKEN_SET_HIT_CANCEL},
  { "$set_whiff_cancel",  TOKEN_SET_WHIFF_CANCEL},
  { "$set_counter",  TOKEN_SET_COUNTER},
  { "$get_meter",  TOKEN_GET_METER},
  { "$add_meter",  TOKEN_ADD_METER},
  { "$subtract_meter",  TOKEN_SUBTRACT_METER},
};

class Scanner {
public:
  Scanner();
  ~Scanner();

  void initScanner(const char* source);
  Token scan();
private:
  
  void skipWhitespace();
  char peek();
  char peekNext();
  char advance();
  bool match(char expected);
  TokenType checkKeyword(int start, int end, const char* rest, TokenType type);
  bool isAtEnd();
  bool isAlpha(char c);
  bool isDigit(char c);

  Token string();
  Token number();
  Token identifier();
  TokenType identifierType();

  Token makeToken(TokenType tokenType);
  Token errorToken(const char* message);

  const char* start;
  const char* current;
  int line;
};

#endif
