#pragma once

#include "VMObject.h"
#include "VMFrame.h"
#include "VMClass.h"
#include <array>
#include <vector>
#include <cstdint>
#include <string>

#define STACK_SIZE 2048
#define MAX_FRAME_COUNT 40

// simple VM. Due to time constraints, ease of implementation is preferred over absolute speed
// also not thread safe.

class VMState;

class VM {
public:
  VM();
  VMValue InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMValue> objects);
private:
  void Execute(VMState &state);
  
  std::array<VMValue, STACK_SIZE> m_stack;
  std::array<VMFrame, MAX_FRAME_COUNT> m_frames;
  

  uint32_t m_stack_ptr;
  uint32_t m_frame_ptr;
};


VM &VMInstance();