#pragma once
#include "Utility/LoggerManager.h"
#include "VM/VMObject.h"
#include "VM/FFI/ConversionFunctions.h"
#include <functional>
#include <type_traits>
#include <array>

#include "VM/FFI/NativeBindingTypedef.h"


// Creates binding for 4-param void function
template <typename ReturnType,
          typename FirstParamType, 
          typename SecondParamType, 
          typename ThirdParameterType,
          typename FourthParameterType,
          typename ClassType,
          typename MemberFunctionPointer,
          typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](VMObject *stack, uint32_t &stack_pointer) {

    auto fourthParam = ToNativeType<FourthParameterType>(stack[--stack_pointer]);
    auto thirdParam = ToNativeType<ThirdParameterType>(stack[--stack_pointer]);
    auto secondParam = ToNativeType<SecondParamType>(stack[--stack_pointer]);
    auto firstParam = ToNativeType<FirstParamType>(stack[--stack_pointer]);

    auto classPointer = ToNativeType<ClassType *>(stack[--stack_pointer]);
    LoggerManager::GetLog("temp.txt").AddLine(LogLevel::INFO, "Greetings from native binding");
    pointer(classPointer, firstParam, secondParam, thirdParam, fourthParam);
  };
}
