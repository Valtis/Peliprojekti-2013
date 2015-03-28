#pragma once
#include <cstdint>
enum class ByteCode : uint32_t { 
  NOP, 
  RETURN, 
  PUSH_INTEGER, 
  STORE_STATIC_OBJECT, 
  LOAD_STATIC_OBJECT,
  STORE_LOCAL,
  LOAD_LOCAL,
  INVOKE_NATIVE, 
  INVOKE_MANAGED, 
  ADD_INTEGER, 
  SUB_INTEGER, 
  MUL_INTEGER, 
  DIV_INTEGER,
  JUMP_IF_ZERO,
  JUMP_IF_NEGATIVE,
  JUMP_IF_POSITIVE, 
  DOUBLE_TO_INTEGER };


const char *GetByteCodeName(ByteCode code);