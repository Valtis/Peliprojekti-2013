#pragma once
#include "VM/VMObject.h"
template<typename T>
T ToNativeType(VMObject obj) {
  static_assert ( std::is_pointer<T>::value, "This type of conversion is not implemented");
  return (T)obj.as_native_pointer();
}

template<>
std::string ToNativeType<std::string>(VMObject obj);

template<>
int32_t ToNativeType<int32_t>(VMObject obj);

template<>
float ToNativeType<float>(VMObject obj);

template<>
double ToNativeType<double>(VMObject obj);

template<>
bool ToNativeType<bool>(VMObject obj);

template<>
char ToNativeType<char>(VMObject obj);


