#pragma once
#include "Utility/LoggerManager.h"
#include "VM/VMValue.h"
#include "VM/FFI/ConversionFunctions.h"
#include <functional>
#include <type_traits>
#include <tuple>

#include "VM/FFI/NativeBindingTypedef.h"
#include "VM/VMOperations.h"


// helper macros hiding the templates. TODO: Check if these can be handled with single variadic macro instead
#define CREATE_2_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM, SECOND_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    CLASS,          \
                    decltype(std::mem_fn(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    SECOND_PARAM>(std::mem_fn(&CLASS::FUNCTION))

#define CREATE_4_ARGS_BINDING(CLASS, FUNCTION, FIRST_PARAM, SECOND_PARAM, THIRD_PARAM, FOURTH_PARAM)  \
CreateNativeBinding<decltype(GetReturnType(&CLASS::FUNCTION)), \
                    CLASS,          \
                    decltype(std::mem_fn(&CLASS::FUNCTION)), \
                    FIRST_PARAM,    \
                    SECOND_PARAM,   \
                    THIRD_PARAM,    \
                    FOURTH_PARAM>(std::mem_fn(&CLASS::FUNCTION))



// thanks to user 'MSN' from Stack Overflow. Slightly modified.
// http://stackoverflow.com/questions/5147492/member-function-call-in-decltype
template <typename R, typename C, typename... A>
R GetReturnType(R(C::*)(A...));


// base case for when whole tuple has been handled - do nothing
template <size_t position, typename Tuple>
void SetTupleValue(Tuple &t, std::vector<VMValue> &stack) {

}


// main recursive template. Writes parameter n value into tuple index n
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




// Thanks to user 'Johannes Schaub - litb' from Stack Overflow. Slightly modified to fit the existing code and naming scheme
// http://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template<int ...>
struct seq { };

template<int N, int ...S>
struct gens : gens < N - 1, N - 1, S... > { };

template<int ...S>
struct gens < 0, S... > {
  typedef seq<S...> type;
};

template<typename Function, typename Pointer, typename Tuple, int ...S>
void CallFunctionImpl(Function f, Pointer p, Tuple params, seq<S...>) {
  f(p, std::get<S>(params) ...);
}

template <typename Function, typename Pointer, typename Tuple, typename... Args>
void CallFunction(Function f, Pointer p, Tuple params) {
  CallFunctionImpl(f, p, params,  typename gens<sizeof...(Args)>::type());
}

///// END THANKS


template <typename ReturnType,
          typename ClassType,
          typename MemberFunctionPointer,
          typename... ParameterTypes,
          typename std::enable_if<std::is_void<ReturnType>::value>::type* = nullptr>
NativeBinding CreateNativeBinding(MemberFunctionPointer memberFunctionPointer) {
  return [=](std::vector<VMValue> &stack) {
    
    auto parameterTuple = ConstructParameterTuple<ParameterTypes...>(stack);
    auto classObjectPointer = ToNativeType<ClassType *>(Op::PopValue(stack));

    CallFunction<decltype(memberFunctionPointer), decltype(classObjectPointer), decltype(parameterTuple), ParameterTypes...>(
      memberFunctionPointer, classObjectPointer, parameterTuple);
  };
}
