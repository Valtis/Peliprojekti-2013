#include "VM/VMState.h"
#include "VM/ByteCode.h"
VMState::VMState(const std::string &path) {
  LoadByteCodeFile(path);
}

void VMState::LoadByteCodeFile(const std::string &path) {
  // TODO: Load from file. Hardcoded for testing purposes for now
  VMFunction function;
  
  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  // this is index to the const object pointer pool managed by the vm.
  function.AddByteCode(static_cast<ByteCode>(123456789));
  
  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(10)); // MessageType::TakeDamage
  
  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(4)); // Priority::Highest

  function.AddByteCode(ByteCode::INVOKE_NATIVE);
  function.AddByteCode(ByteCode::RETURN);

  m_functions["initialize"] = function;
}

VMFunction *VMState::GetFunction(const std::string &name) {
  auto iter = m_functions.find(name);
  if (iter == m_functions.end()) {
    return nullptr;
  }

  return &iter->second;
}