#pragma once
#include "VM/VMValue.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/VariadicFunctionCallTemplates.h"
#include "VM/FFI/FunctionArgumentTupleTemplates.h"
#include "VM/FFI/NativeBindingTypedef.h"
#include "VM/VMOperations.h"

#include <functional>
#include <type_traits>
#include <tuple>

// helper macro for registering native binding
#define CREATE_NATIVE_CLASS_BINDING(CLASS, FUNCTION,  ...) \
CreateNativeBindingForMemberFunction<decltype(GetMemberReturnType(&CLASS::FUNCTION)), \
                    CLASS,    \
                    decltype(std::mem_fn(&CLASS::FUNCTION)),  \
                    __VA_ARGS__>(std::mem_fn(&CLASS::FUNCTION))

template <typename ReturnType, typename... Args,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateBinding(ReturnType(*ptr)(Args...)) {
  return [=](std::vector<VMValue> &stack) {
    auto parameterTuple = ConstructParameterTuple<Args...>(stack);
    CallFreeFunction<decltype(ptr), decltype(parameterTuple), Args...>(
      ptr, parameterTuple);
  };
}

template <typename Class, typename ReturnType, typename... Args,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateBinding(ReturnType (Class::*ptr)(Args...)) {

  return [=](std::vector<VMValue> &stack) {
    auto functionPtr = std::mem_fn(ptr);
    auto classObjectPointer = ToNativeType<Class *>(Op::PopValue(stack));
    auto parameterTuple = ConstructParameterTuple<Args...>(stack);

    CallMemberFunction<decltype(functionPtr), decltype(classObjectPointer), decltype(parameterTuple), Args...>(functionPtr, 
      classObjectPointer, parameterTuple);
  };
}
