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
  function.SetName("initialize");
  
  std::string name = "RegisterMessageHandler";
  VMValue obj = MemMgrInstance().AllocateArray(ValueType::CHAR, name.length());
  MemMgrInstance().WriteToArrayIndex(obj, &name[0], 0, name.length());
  m_permanent_storage.push_back(obj);

  name = "OnDamageTaken";
  obj = MemMgrInstance().AllocateArray(ValueType::CHAR, name.length());
  MemMgrInstance().WriteToArrayIndex(obj, &name[0], 0, name.length());
  m_permanent_storage.push_back(obj);

  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(10)); // MessageType::TakeDamage
  
  function.AddByteCode(ByteCode::PUSH_INTEGER);
  function.AddByteCode(static_cast<ByteCode>(4)); // Priority::Highest
  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  // index to const object pool
  function.AddByteCode(static_cast<ByteCode>(1));

  function.AddByteCode(ByteCode::PUSH_CONSTANT_OBJECT);
  // index to const object pool
  function.AddByteCode(static_cast<ByteCode>(0));

  function.AddByteCode(ByteCode::INVOKE_NATIVE);
  function.AddByteCode(ByteCode::RETURN);

  m_functions["initialize"] = function;

  VMFunction damageTaken;
  damageTaken.SetName("OnDamageTaken");
  damageTaken.AddByteCode(ByteCode::PUSH_INTEGER);
  damageTaken.AddByteCode(static_cast<ByteCode>(0)); // MessagePassing::PASS_FORWARD
  damageTaken.AddByteCode(ByteCode::RETURN);

  m_functions["OnDamageTaken"] = damageTaken;
}



const VMFunction *VMState::GetFunction(const std::string &name) const {
  auto iter = m_functions.find(name);
  if (iter == m_functions.end()) {
    return nullptr;
  }

  return &iter->second;
}

VMValue VMState::GetPermanentStorageObject(uint32_t index) const {
  return m_permanent_storage.at(index);
}

NativeBinding VMState::GetNativeBinding(const std::string &name) const {
  auto bindIter = m_native_bindings.find(name);
  if (bindIter == m_native_bindings.end()) {
    throw std::runtime_error(std::string("No native binding with name ") + name + " has been registered");
  }
  return bindIter->second;
}

void VMState::AddNativeBinding(const std::string &name, NativeBinding binding) {
  m_native_bindings[name] = binding;
}