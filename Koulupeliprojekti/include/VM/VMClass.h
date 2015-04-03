#pragma once

#include "VM/VMValue.h"
#include "VM/VMFunction.h"
#include <vector>
#include <cstdint>
class VMClass {
public:
  uint32_t GetFieldCount();
  uint32_t GetFunctionCount();


  void SetFields(std::vector<ValueType> fields);
  void SetFunctions(std::vector<VMFunction> functions);
  ValueType GetFieldType(uint32_t field_index);
  VMFunction *GetFunction(uint32_t function_index);

  
private:
  std::vector<ValueType> m_fields;
  std::vector<VMFunction> m_functions;
};