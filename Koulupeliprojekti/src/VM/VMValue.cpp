#include "VM/VMValue.h"
uint32_t TypeSize(ValueType type) {
  switch (type) {
  case ValueType::UNINITIALIZED:
    return 0;
  case ValueType::INT:
    return sizeof(int32_t);
  case ValueType::FLOAT:
    return sizeof(float);
  case ValueType::DOUBLE:
    return sizeof(double);
  case ValueType::BOOL:
    return sizeof(bool);
  case ValueType::CHAR:
    return sizeof(char);
  case ValueType::NATIVE_POINTER:
    return sizeof(void *);
  case ValueType::MANAGED_POINTER:
  default:
    return 0;
  }
}


std::string VMValue::to_string() const {
   std::string str;
   switch (m_type) {
   case ValueType::INT:
     str = "Integer: " + std::to_string(m_value.int_value);
     break;
   case ValueType::FLOAT:
     str = "Float: " + std::to_string(m_value.float_value);
     break;
   case ValueType::DOUBLE:
     str = "Double: " + std::to_string(m_value.double_value);
     break;
   case ValueType::BOOL:
     str = "Boolean: " + std::to_string(m_value.bool_value);
     break;
   case ValueType::CHAR:
     str = "Char: " + std::to_string(m_value.char_value);
     break;
   case ValueType::NATIVE_POINTER:
   {
     std::ostringstream stream;
     stream << "0x"
       << std::setfill('0') << std::setw(sizeof(void *) * 2)
       << std::hex << m_value.native_pointer_value;
     str = "Native pointer: " + stream.str();

   }

   break;
   case ValueType::MANAGED_POINTER:
     str = "Managed pointer: " + std::to_string(m_value.managed_pointer_value);
     break;

   default:
     str = "Unknown type";
   }
   return str;
 }