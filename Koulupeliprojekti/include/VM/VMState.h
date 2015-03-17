#pragma once

#include "VMObject.h"
#include "VMFunction.h"
#include <unordered_map>
#include <vector>
#include <string>

class VMFunction;

class VMState {
public:

  VMState(const std::string &path);

  void LoadByteCodeFile(const std::string &path);
  VMFunction *GetFunction(const std::string &name); 
  VMObject GetPermanentStorageObject(uint32_t index);

private:
  std::unordered_map<std::string, VMFunction> m_functions;
  std::vector<VMObject> m_permanent_storage;
};