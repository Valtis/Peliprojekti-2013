#include "VM/VMFunction.h"

ByteCode VMFunction::GetByteCode(uint32_t pos) {
  return m_bytecode.at(pos);
}