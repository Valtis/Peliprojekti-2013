#pragma once
#include "VM/VMObject.h"
#include <functional>
typedef std::function<void(VMObject *stack, uint32_t &stack_pointer)> NativeBinding;