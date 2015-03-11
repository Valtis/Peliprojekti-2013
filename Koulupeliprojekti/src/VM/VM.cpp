#include "VM/VM.h"
#include "VM/VMState.h"
#include "Utility/LoggerManager.h"

void VM::InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMObject> objects) {
  

  auto &log = LoggerManager::GetLog(VM_LOG);
  log.AddLine(LogLevel::DEBUG, "Attempting to invoke script function " + functionName);

  int i = 0;
  for (const auto &o : objects) {
    log.AddLine(LogLevel::DEBUG, "Parameter " + std::to_string(++i) + ": " + o.to_string());
  }

  auto function = state.GetFunction(functionName);
  if (function == nullptr) {
    log.AddLine(LogLevel::DEBUG, "No function called " + functionName + " found - aborting");  
    return;
  }
  
  m_stack_ptr = 0;
  m_frame_ptr = 0;

  m_frames[m_frame_ptr] = VMFrame{ function };

  for (const auto &o : objects) {
    m_stack[m_stack_ptr++] = o;
  }

  Execute();
}


void VM::Execute() {
}




VM &VMInstance() {
  static VM vm;
  return vm;
}