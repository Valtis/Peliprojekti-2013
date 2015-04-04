#include "VM/VMState.h"
#include "VM/ByteCode.h"
#include "VM/VM.h"
#include "VM/ScriptLoader.h"
VMState::VMState(const std::string &path) {
  LoadByteCodeFile(path);
}


VMState::~VMState() {
  VMInstance().UnregisterVMState(this);
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

void VMState::SetStaticObject(uint32_t index, VMValue value) {
  m_static_objects.at(index) = value;
}

VMValue VMState::GetStaticObject(uint32_t index) const {
  return m_static_objects.at(index);
}

VMValue &VMState::GetStaticObjectReference(uint32_t index) {
  return m_static_objects.at(index);
}

size_t VMState::AddStaticObject(VMValue value) {
  m_static_objects.push_back(value);
  return m_static_objects.size() - 1;
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


void VMState::AddFunction(const std::string &name, VMFunction function) {
  m_functions[name] = function;
}

size_t VMState::GetStaticObjectCount() const {
  return m_static_objects.size();
}