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
 #define CREATE_NATIVE_BINDING(CLASS, FUNCTION,  ...) \
CreateNativeBindingForMemberFunction<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    CLASS,    \
                    decltype(std::mem_fn(&CLASS::FUNCTION)),  \
                    __VA_ARGS__>(std::mem_fn(&CLASS::FUNCTION))


// thanks to user 'MSN' from Stack Overflow. Slightly modified.
// http://stackoverflow.com/questions/5147492/member-function-call-in-decltype
template <typename R, typename C, typename... A>
R GetReturnType(R(C::*)(A...));


template <typename ReturnType,
          typename ClassType,
          typename MemberFunctionPointer,
          typename... ParameterTypes,
          typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateNativeBindingForMemberFunction(MemberFunctionPointer memberFunctionPointer) {
  return [=](std::vector<VMValue> &stack) {

    auto classObjectPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    auto parameterTuple = ConstructParameterTuple<ParameterTypes...>(stack);

    CallMemberFunction<decltype(memberFunctionPointer), decltype(classObjectPointer), decltype(parameterTuple), ParameterTypes...>(
      memberFunctionPointer, classObjectPointer, parameterTuple);
  };
}

template <typename ReturnType,
  typename ClassType,
  typename MemberFunctionPointer,
  typename... ParameterTypes,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBindingForFreeFunction(MemberFunctionPointer memberFunctionPointer) {
  return [=](std::vector<VMValue> &stack) {

    auto parameterTuple = ConstructParameterTuple<ParameterTypes...>(stack);

    CallFreeFunction<decltype(memberFunctionPointer), decltype(parameterTuple), ParameterTypes...>(
      memberFunctionPointer, classObjectPointer, parameterTuple);
  };
}