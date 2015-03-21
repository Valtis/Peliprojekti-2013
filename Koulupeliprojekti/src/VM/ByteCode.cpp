#include "VM/ByteCode.h"


const char *GetByteCodeName(ByteCode code) {
  switch (code) {
    case ByteCode::NOP:
      return "NOP";
    case ByteCode::RETURN:
      return "RETURN";
    case ByteCode::PUSH_INTEGER:
      return "PUSH_INTEGER";
    case ByteCode::PUSH_CONSTANT_OBJECT:
      return "PUSH_CONSTANT_OBJECT";
    case ByteCode::INVOKE_NATIVE:
      return "INVOKE_NATIVE";
    default:
      return "Unknown code";
  }
}
