#include "VM/VMOperations.h"
#include "VM/VMValue.h"
#include "VM/VMFrame.h"
#include "VM/VMState.h"
#include "VM/VMFunction.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/NativeBinding.h"

#include "Utility/LoggerManager.h"
#include "VM/MemoryManager.h"
#include <vector>

namespace Op {

  void PushValue(const VMValue &value, std::vector<VMValue> &stack) {
    stack.push_back(value);
  }

  VMValue PopValue(std::vector<VMValue> &stack) {
    auto value = stack.back();
    stack.pop_back();
    return value;
  }

  void PushInteger(std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    PushValue(VMValue{ (int32_t)frames.back().GetNextInstruction() }, stack);
  }


  void LoadStaticObject(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = (uint32_t)frames.back().GetNextInstruction();
    PushValue(state.GetStaticObject(index), stack);
  }

  void StoreStaticObject(VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = (uint32_t)frames.back().GetNextInstruction();
    auto value = PopValue(stack);
    state.SetStaticObject(index, value);
  }

  void AddInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first + second }, stack);
  }

  void SubInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first - second }, stack);
  }

  void MulInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first * second }, stack);
  }

  void DivInteger(std::vector<VMValue> &stack) {
    auto second = PopValue(stack).as_int();
    auto first = PopValue(stack).as_int();
    PushValue(VMValue{ first / second }, stack);
  }

  void InvokeNative(const VMState &state, std::vector<VMValue> &stack) {
    auto ptrToStr = PopValue(stack);
    LoggerManager::GetLog(VM_LOG).AddLine(LogLevel::INFO, "Invoking native function " + ToNativeType<std::string>(ptrToStr));
    auto binding = state.GetNativeBinding(ToNativeType<std::string>(ptrToStr));
    binding(stack);
  }

  void InvokeManaged(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto ptrToName = PopValue(stack);
    auto name = ToNativeType<std::string>(ptrToName);
    auto function = state.GetFunction(name);
    if (function == nullptr) {
      throw std::runtime_error("Could not invoke function " + name + ": No such function exists");
    }
    frames.push_back(function);
  }


  bool Return(std::vector<VMFrame> &frames) {
    frames.pop_back();
    if (frames.empty())  {
      return false;
    }
    return true;

  }

  void JumpIfZero(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = (uint32_t)frames.back().GetNextInstruction();
    auto value = PopValue(stack);

    if (value.as_int() == 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void JumpIfNegative(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = (uint32_t)frames.back().GetNextInstruction();
    auto value = PopValue(stack);
    if (value.as_int() < 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void JumpIfPositive(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto jumpDestination = (uint32_t)frames.back().GetNextInstruction();
    auto value = PopValue(stack);
    if (value.as_int() > 0) {
      frames.back().SetNextInstruction(jumpDestination);
    }
  }

  void StoreLocal(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = (uint32_t)frames.back().GetNextInstruction();
    auto value = PopValue(stack);

    frames.back().SetLocalVariable(index, value);
  }

  void LoadLocal(const VMState &state, std::vector<VMValue> &stack, std::vector<VMFrame> &frames) {
    auto index = (uint32_t)frames.back().GetNextInstruction();
    PushValue(frames.back().GetLocalVariable(index), stack);
  }

  void DoubleToInteger(std::vector<VMValue> &stack) {
    auto value = PopValue(stack);
    value.set_int((int32_t)value.as_double());
    PushValue(value, stack);
  }

  void AllocateIntegerArray(std::vector<VMValue> &stack) {
    auto size = PopValue(stack);
    PushValue(MemMgrInstance().AllocateArray(ValueType::INT, size.as_int()), stack);
  }
}