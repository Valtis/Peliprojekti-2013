#pragma once
#include "Utility/LoggerManager.h"
#include "VM/VMValue.h"
#include "VM/FFI/ConversionFunctions.h"
#include <functional>
#include <type_traits>
#include <array>

#include "VM/FFI/NativeBindingTypedef.h"
#include "VM/VMOperations.h"

// thanks to MSN from stackoverflow. Slightly modified.
// http://stackoverflow.com/questions/5147492/member-function-call-in-decltype
template <typename R, typename C, typename... A>
R GetReturnType(R(C::*)(A...));




#define CREATE_4_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM, SECOND_PARAM, THIRD_PARAM, FOURTH_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    SECOND_PARAM,   \
                    THIRD_PARAM,    \
                    FOURTH_PARAM,   \
                    CLASS>(std::mem_fn(&CLASS::FUNCTION))



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
  return [=](std::vector<VMValue> &stack) {

    auto fourthParam = ToNativeType<FourthParameterType>(Op::PopValue(stack));
    auto thirdParam = ToNativeType<ThirdParameterType>(Op::PopValue(stack));
    auto secondParam = ToNativeType<SecondParamType>(Op::PopValue(stack));
    auto firstParam = ToNativeType<FirstParamType>(Op::PopValue(stack));

    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    LoggerManager::GetLog("temp.txt").AddLine(LogLevel::INFO, "Greetings from native binding");
    pointer(classPointer, firstParam, secondParam, thirdParam, fourthParam);
  };
}
