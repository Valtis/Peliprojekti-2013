#pragma once
#include "Utility/LoggerManager.h"
#include "VM/VMValue.h"
#include "VM/FFI/ConversionFunctions.h"
#include <functional>
#include <type_traits>
#include <tuple>

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



// base case for when whole tuple has been handled - do nothing
template <size_t position, typename Tuple>
void SetTupleValue(Tuple &t, std::vector<VMValue> &stack) {

}

template<size_t position, typename Tuple, typename ParameterType, typename ...RemainingParameterTypes>
void SetTupleValue(Tuple &t, std::vector<VMValue> &stack) {
  SetTupleValue<position + 1, decltype(t), RemainingParameterTypes...>(t, stack);
  std::get<position>(t) = ToNativeType<ParameterType>(Op::PopValue(stack));
}

template<typename ...ParameterTypes>
std::tuple<ParameterTypes...> ConstructParameterTuple(std::vector<VMValue> &stack) {
  std::tuple<ParameterTypes...> t;
  SetTupleValue<0, decltype(t), ParameterTypes...>(t, stack);
  return t;
}



// Creates binding for 4-param void function
template <typename ReturnType,
          typename FirstParamType, 
          typename SecondParamType, 
          typename ThirdParamType,
          typename FourthParamType,
          typename ClassType,
          typename MemberFunctionPointer,
          typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateNativeBinding(MemberFunctionPointer pointer) {
  return [=](std::vector<VMValue> &stack) {
    
    auto parameterTuple = ConstructParameterTuple<FirstParamType, SecondParamType, ThirdParamType, FourthParamType>(stack);
    auto classPointer = ToNativeType<ClassType *>(Op::PopValue(stack));
    pointer(classPointer, std::get<0>(parameterTuple), std::get<1>(parameterTuple), std::get<2>(parameterTuple), std::get<3>(parameterTuple));
  };
}
