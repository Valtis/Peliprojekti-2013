#pragma once
#include "VMFunction.h"
#include "VMObject.h"
#include <cstdint>
class VMFrame {

public:
  VMFrame();
  VMFrame(VMFunction *function);
  ByteCode GetNextInstruction();
  void SetNextInstruction(uint32_t);
private:
  std::vector<VMObject> m_local_variables;
  VMFunction *m_function; // non-owning pointer - pointed value must not be deleted
  uint32_t m_current_instruction;
};