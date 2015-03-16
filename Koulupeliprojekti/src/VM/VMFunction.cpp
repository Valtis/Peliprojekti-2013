#include "VM/VMFunction.h"

void VMFunction::AddByteCode(ByteCode code) {
  m_bytecode.push_back(code);
}

ByteCode VMFunction::GetByteCode(uint32_t pos) const {
  return m_bytecode.at(pos);
}