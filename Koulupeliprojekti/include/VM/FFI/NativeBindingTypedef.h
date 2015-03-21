#pragma once
#include "VM/VMValue.h"
#include <functional>
typedef std::function<void(VMValue *stack, uint32_t &stack_pointer)> NativeBinding;