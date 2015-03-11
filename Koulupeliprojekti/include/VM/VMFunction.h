#pragma once
#include "ByteCode.h"
#include <vector>
#include <cstdint>


class VMFunction {
public:

  ByteCode GetByteCode(uint32_t pos);
private:

  std::vector<ByteCode> m_bytecode;

};
