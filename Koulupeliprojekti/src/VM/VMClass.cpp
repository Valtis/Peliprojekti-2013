#include "VM/VMClass.h"

void VMClass::SetFields(std::vector<ValueType> fields) {
  m_fields = fields;
}

void VMClass::SetFunctions(std::vector<VMFunction> functions) {
  m_functions = functions;
}

ValueType VMClass::GetFieldType(uint32_t field_index) {
  return m_fields.at(field_index);
}

VMFunction *VMClass::GetFunction(uint32_t function_index) {
  return &m_functions.at(function_index);
}
