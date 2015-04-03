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
  ~VMState();
  void LoadByteCodeFile(const std::string &path);
  const VMFunction *GetFunction(const std::string &name) const; 
 
  void SetStaticObject(uint32_t index, VMValue value);
  VMValue GetStaticObject(uint32_t index) const;
  VMValue &GetStaticObjectReference(uint32_t index);
  size_t AddStaticObject(VMValue value);
  
  NativeBinding GetNativeBinding(const std::string &name) const;
  void AddNativeBinding(const std::string &name, NativeBinding binding);

  void AddFunction(const std::string &name, VMFunction function);


  size_t GetStaticObjectCount() const;

private:
  std::unordered_map<std::string, VMFunction> m_functions;
  std::unordered_map<std::string, NativeBinding> m_native_bindings;
  std::vector<VMValue> m_static_objects;
};