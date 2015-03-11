#pragma once

#include "VMFunction.h"
#include <unordered_map>
#include <string>

class VMFunction;

class VMState {
public:

  void LoadByteCodeFile(const std::string &path);
  VMFunction *GetFunction(const std::string &name);

private:
  std::unordered_map<std::string, VMFunction> m_functions;

};