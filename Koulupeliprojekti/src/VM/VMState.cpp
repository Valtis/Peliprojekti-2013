#include "VM/VMState.h"


void VMState::LoadByteCodeFile(const std::string &path) {

}

VMFunction *VMState::GetFunction(const std::string &name) {
  auto iter = m_functions.find(name);
  if (iter == m_functions.end()) {
    return nullptr;
  }

  return &iter->second;
}