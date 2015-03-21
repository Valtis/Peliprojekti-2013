#pragma once

#include "VMValue.h"
#include "VMFunction.h"
#include <unordered_map>
#include <vector>
#include <string>

#include "VM/FFI/NativeBindingTypedef.h"

class VMFunction;

class VMState {
public:

  VMState(const std::string &path);

  void LoadByteCodeFile(const std::string &path);
  const VMFunction *GetFunction(const std::string &name) const; 
  VMValue GetPermanentStorageObject(uint32_t index) const;
  
  NativeBinding GetNativeBinding(const std::string &name) const;
  void AddNativeBinding(const std::string &name, NativeBinding binding);

private:
  std::unordered_map<std::string, VMFunction> m_functions;
  std::unordered_map<std::string, NativeBinding> m_native_bindings;
  std::vector<VMValue> m_permanent_storage;
};