#include "VM/ByteCode.h"


const char *GetByteCodeName(ByteCode code) {
  switch (code) {
    case ByteCode::NOP:
      return "NOP";
    case ByteCode::RETURN:
      return "RETURN";
    case ByteCode::PUSH_INTEGER:
      return "PUSH_INTEGER";
    case ByteCode::STORE_STATIC_OBJECT:
      return "STORE_STATIC_OBJECT";
    case ByteCode::LOAD_STATIC_OBJECT:
      return "LOAD_STATIC_OBJECT";
    case ByteCode::STORE_LOCAL:
      return "STORE_LOCAL";
    case ByteCode::LOAD_LOCAL:
      return "LOAD_LOCAL";
    case ByteCode::INVOKE_NATIVE:
      return "INVOKE_NATIVE";
    case ByteCode::INVOKE_MANAGED:
      return "INVOKE_MANAGED";
    case ByteCode::ADD_INTEGER:
      return "ADD_INTEGER";
    case ByteCode::SUB_INTEGER:
      return "SUB_INTEGER";
    case ByteCode::MUL_INTEGER:
      return "MUL_INTEGER";
    case ByteCode::DIV_INTEGER:
      return "DIV_INTEGER";
    case ByteCode::JUMP_IF_ZERO:
      return "JUMP_IF_ZERO";
    case ByteCode::JUMP_IF_NEGATIVE:
      return "JUMP_IF_NEGATIVE";
    case ByteCode::JUMP_IF_POSITIVE:
      return "Jump_IF_POSITIVE";
    case ByteCode::DOUBLE_TO_INTEGER:
      return "DOUBLE_TO_INTEGER";
    default:
      return "Unknown code";
  }
}