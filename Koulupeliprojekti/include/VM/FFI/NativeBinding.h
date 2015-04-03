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


// helper macro for registering native binding
#define CREATE_NATIVE_FREE_BINDING(FUNCTION,  ...) \
CreateNativeBindingForFreeFunction<GetFreeReturnType<decltype(&FUNCTION)>::type, \
                    decltype(&FUNCTION), \
                    __VA_ARGS__>(&FUNCTION)

// thanks to user 'MSN' from Stack Overflow. Slightly modified.
// http://stackoverflow.com/questions/5147492/member-function-call-in-decltype
template <typename R, typename C, typename... A>
R GetMemberReturnType(R(C::*)(A...));

// thanks to user 'Angew' from Stack Overflow. Naming modified to fit existing naming scheme 
// http://stackoverflow.com/questions/18695564/deducing-a-function-pointer-return-type
template <class F>
struct GetFreeReturnType;

template <class R, class... A>
struct GetFreeReturnType < R(*)(A...) >
{
  typedef R type;
};

// end thanks to Angew

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
  typename FunctionPointer,
  typename... ParameterTypes,
  typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
  NativeBinding CreateNativeBindingForFreeFunction(FunctionPointer functionPointer) {
  return [=](std::vector<VMValue> &stack) {

    auto parameterTuple = ConstructParameterTuple<ParameterTypes...>(stack);
    CallFreeFunction<decltype(functionPointer), decltype(parameterTuple), ParameterTypes...>(
      functionPointer, parameterTuple);
  };
}