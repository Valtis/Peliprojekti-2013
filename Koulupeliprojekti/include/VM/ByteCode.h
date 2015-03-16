#pragma once
#include <cstdint>
enum class ByteCode : uint32_t { NOP, RETURN, PUSH_INTEGER, PUSH_CONSTANT_OBJECT, INVOKE_NATIVE };
