#include "VM/VMState.h"
#include "VM/ByteCode.h"
#include "VM/VM.h"
#include "VM/MemoryManager.h"
#include "VM/ScriptLoader.h"
VMState::VMState(const std::string &path) {
  LoadByteCodeFile(path);
}



void VMState::LoadByteCodeFile(const std::string &path) {

  ScriptLoader loader(*this, path);
  loader.Load();
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

size_t VMState::AddPermanentObject(VMValue value) {
  m_permanent_storage.push_back(value);
  return m_permanent_storage.size() - 1;
}

void VMState::AddFunction(const std::string &name, VMFunction function) {
  m_functions[name] = function;
}