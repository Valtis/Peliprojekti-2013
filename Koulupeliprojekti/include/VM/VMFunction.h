#pragma once
#include "ByteCode.h"
#include <vector>
#include <cstdint>


class VMFunction {
public:
  void VMFunction::AddByteCode(ByteCode code);
  ByteCode GetByteCode(uint32_t pos) const;
private:

  std::vector<ByteCode> m_bytecode;

};
