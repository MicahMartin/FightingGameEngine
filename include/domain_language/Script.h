#ifndef _Script_h
#define _Script_h

#include <vector>
#include <unordered_map>
#include "domain_language/Common.h"
#include "domain_language/Value.h"
#include "domain_language/Scanner.h"

typedef enum {
  OP_CONSTANT,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_POP,
  OP_GET_LOCAL,
  OP_SET_LOCAL,
  OP_DEFINE_GLOBAL,
  OP_SET_GLOBAL,
  OP_GET_GLOBAL,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NOT,
  OP_NEGATE,
  OP_PRINT,
  OP_JUMP_IF_FALSE,
  OP_JUMP,
  OP_LOOP,
  OP_GET_ANIM_TIME,
  OP_GET_HIT_STUN,
  OP_GET_BLOCK_STUN,
  OP_SET_BLOCK_STUN,
  OP_GET_STATE_TIME,
  OP_GET_Y_POS,
  OP_GET_INPUT,
  OP_GET_STATE_NUM,
  OP_GET_CONTROL,
  OP_WAS_PRESSED,
  OP_GET_COMBO,
  OP_GET_IS_ALIVE,
  OP_GET_HIT_CANCEL,
  OP_GET_WHIFF_CANCEL,
  OP_HAS_AIR_ACTION,
  OP_CHANGE_STATE,
  OP_CANCEL_STATE,
  OP_VELSET_X,
  OP_NEG_VELSET_X,
  OP_VELSET_Y,
  OP_MOVE_F,
  OP_MOVE_B,
  OP_MOVE_U,
  OP_MOVE_D,
  OP_SET_CONTROL,
  OP_SET_COMBO,
  OP_SET_GRAVITY,
  OP_SET_NOGRAV_COUNT,
  OP_SET_AIR_ACTION,
  OP_SET_HIT_STUN,
  OP_SET_HIT_CANCEL,
  OP_SET_WHIFF_CANCEL,
  OP_SET_COUNTER,
  OP_RESET_ANIM,
  OP_GET_ENTITY_STATUS,
  OP_ACTIVATE_ENTITY,
  OP_DEACTIVATE_ENTITY,
  OP_SNAP_TO_OPPONENT,
  OP_CHECK_COMMAND,
  OP_GET_METER,
  OP_ADD_METER,
  OP_SUBTRACT_METER,
  OP_GET_INSTALL,
  OP_SET_INSTALL,
  OP_GET_Y_VEL,
  OP_GET_X_VEL,
  OP_RETURN
} OpCode;

static const std::unordered_map<std::string, OpCode> engineCallMap {
  // modify stack
  {"$get_anim_time", OP_GET_ANIM_TIME},
  {"$get_hit_stun", OP_GET_HIT_STUN},
  {"$get_block_stun", OP_GET_BLOCK_STUN},
  {"$set_block_stun", OP_SET_BLOCK_STUN},
  {"$get_state_time", OP_GET_STATE_TIME},
  {"$get_y_pos", OP_GET_Y_POS},
  {"$get_input", OP_GET_INPUT},
  {"$get_state_num", OP_GET_STATE_NUM},
  {"$get_control", OP_GET_CONTROL},
  {"$was_pressed", OP_WAS_PRESSED},
  {"$get_combo", OP_GET_COMBO},
  {"$get_is_alive", OP_GET_IS_ALIVE},
  {"$has_air_action", OP_HAS_AIR_ACTION},
  // modify game state
  {"$change_state", OP_CHANGE_STATE},
  {"$cancel_state", OP_CANCEL_STATE},
  {"$velset_x", OP_VELSET_X},
  {"$neg_velset_x", OP_NEG_VELSET_X},
  {"$velset_y", OP_VELSET_Y},
  {"$move_f", OP_MOVE_F},
  {"$move_b", OP_MOVE_B},
  {"$move_u", OP_MOVE_U},
  {"$move_d", OP_MOVE_D},
  {"$set_control", OP_SET_CONTROL},
  {"$set_combo", OP_SET_COMBO},
  {"$set_gravity", OP_SET_GRAVITY},
  {"$set_nograv_count", OP_SET_NOGRAV_COUNT},
  {"$set_air_action", OP_SET_AIR_ACTION},
  {"$set_hit_stun", OP_SET_HIT_STUN},
  {"$reset_anim", OP_RESET_ANIM},
  {"$activate_entity", OP_ACTIVATE_ENTITY},
  {"$deactivate_entity", OP_DEACTIVATE_ENTITY},
  {"$get_entity_status", OP_GET_ENTITY_STATUS},
  {"$snap_to_opponent", OP_SNAP_TO_OPPONENT},
  {"$check_command", OP_CHECK_COMMAND},
  {"$get_hit_cancel",  OP_GET_HIT_CANCEL},
  {"$get_whiff_cancel", OP_GET_WHIFF_CANCEL},
  {"$set_hit_cancel",  OP_SET_HIT_CANCEL},
  {"$set_whiff_cancel", OP_SET_WHIFF_CANCEL},
  {"$set_counter", OP_SET_COUNTER},
  {"$get_meter", OP_GET_METER},
  {"$add_meter", OP_ADD_METER},
  {"$get_install", OP_GET_INSTALL},
  {"$set_install", OP_SET_INSTALL},
  {"$get_yvel", OP_GET_Y_VEL},
  {"$get_xvel", OP_GET_X_VEL},
  {"$subtract_meter", OP_SUBTRACT_METER},
};

struct Local {
  Token name;
  int depth;
};

class Script {
public:
  Script();
  ~Script();

  void writeByte(uint8_t byte, int lineNumber);
  uint8_t writeSymbol(Value value);

  // return a pointer to the start of the script
  uint8_t* scriptStart();
  // debugging
  // TODO: abstract to template based debugger
  int jumpInstruction(const char* name, int sign, int offset);
  int byteInstruction(const char* name, int offset);
  int symbolInstruction(const char* name, int offset);
  int simpleInstruction(const char* name, int offset);

  int disassembleInstruction(int offset);
  void disassembleScript(const char* name);

  std::vector<Value> symbols;
  std::unordered_map<std::string, Value> globals;
  std::vector<int> lines;

  // TODO: Stop being lazy and make accessor funcs
  std::vector<uint8_t> code;
  Local locals[256];
  int localCount = 0;
  int scopeDepth = 0;
private:
};

#endif /*  */
