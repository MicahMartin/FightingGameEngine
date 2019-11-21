#ifndef _Value_h
#define _Value_h

#include "domain_language/Common.h"
#include <string>

typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_STRING
} ValueType;

struct Value {
  ValueType type;
  union {
    bool boolean;
    long number;
    std::string* string;
  } as;
};

//TODO: Learn templates :(

#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)
#define IS_STRING(value)  ((value).type == VAL_STRING)

#define AS_STRING(value)     ((value).as.string)
#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)

#define STRING_VAL(value)   ((Value){ VAL_STRING, { .string = value } })
#define BOOL_VAL(value)   ((Value){ VAL_BOOL, { .boolean = value } })
#define NIL_VAL           ((Value){ VAL_NIL, { .number = 0 } })
#define NUMBER_VAL(value) ((Value){ VAL_NUMBER, { .number = value } })

namespace ValueFn {
  void printValue(Value value);
}
#endif
