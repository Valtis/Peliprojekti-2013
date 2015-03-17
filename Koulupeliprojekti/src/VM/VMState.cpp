#include "VM/VMState.h"
#include "VM/ByteCode.h"
#include "VM/VM.h"
#include "VM/MemoryManager.h"
VMState::VMState(const std::string &path) {
  LoadByteCodeFile(path);
}

void VMState::LoadByteCodeFile(const std::string &path) {
  // TODO: Load from file. Hardcoded for testing purposes for now
  VMFunction function;
  
  std::string name = "RegisterMessageHandler";
  VMObject obj = MemMgrInstance().AllocateArray(ObjectType::CHAR, name.length());

  for (size_t i = 0; i < name.length(); ++i) {
    MemMgrInstance().WriteToArrayIndex(obj, i, &name[0] + i);
  }
  m_permanent_storage.push_back(obj);

  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  // index to const object pool
  function.AddByteCode(static_cast<ByteCode>(0));
  
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

VMObject VMState::GetPermanentStorageObject(uint32_t index) {
  return m_permanent_storage.at(index);
}