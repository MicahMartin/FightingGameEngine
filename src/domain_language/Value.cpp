#include "domain_language/Value.h"

void ValueFn::printValue(Value value){
  switch (value.type) {
    case VAL_BOOL:   printf(AS_BOOL(value) ? "true" : "false"); break;
    case VAL_NIL:    printf("nil"); break;
    case VAL_NUMBER: printf("%ld", AS_NUMBER(value)); break;
    case VAL_STRING: printf("%s", AS_STRING(value)->c_str()); break;
  }
}
