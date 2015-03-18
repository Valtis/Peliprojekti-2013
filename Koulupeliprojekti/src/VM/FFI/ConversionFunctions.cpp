#include "VM/FFI/ConversionFunctions.h"
#include "VM/MemoryManager.h"
#include <string>
template<>
std::string ToNativeType<std::string>(VMObject obj) {
   std::string str;
   auto length = MemMgrInstance().GetArrayLength(obj);
   str.resize(length);
   MemMgrInstance().ReadFromArrayIndex(obj, &str[0], 0 , length);
   return str;
}

template<> 
int32_t ToNativeType<int32_t>(VMObject obj) {
   return obj.as_int();
}

template<>
float ToNativeType<float>(VMObject obj) {
  return obj.as_float();
}

template<>
double ToNativeType<double>(VMObject obj) {
  return obj.as_double();
}

template<>
bool ToNativeType<bool>(VMObject obj) {
  return obj.as_bool();
}

template<>
char ToNativeType<char>(VMObject obj) {
  return obj.as_char();
}

template<>
void *ToNativeType<void *>(VMObject obj) {
  return obj.as_native_pointer();
}
