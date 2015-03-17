#include "VM/VM.h"
#include "VM/VMState.h"
#include "Utility/LoggerManager.h"
#include "VM/MemoryManager.h"
VM::VM() {
}


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

  Execute(state);
}


void VM::Execute(VMState &state) {

  for (;;)  {
    auto code = m_frames[m_frame_ptr].GetNextInstruction();

    // TODO: Clean up
    switch (code) {

    case ByteCode::PUSH_INTEGER:
      m_stack[m_stack_ptr++] = VMObject{ (int32_t)m_frames[m_frame_ptr].GetNextInstruction() };
      break;
    case ByteCode::PUSH_CONSTANT_OBJECT: 
      m_stack[m_stack_ptr++] = state.GetPermanentStorageObject((int32_t)m_frames[m_frame_ptr].GetNextInstruction());
      break;

    case ByteCode::INVOKE_NATIVE:
      {
        VMObject second_param = m_stack[--m_stack_ptr];
        VMObject first_param = m_stack[--m_stack_ptr];
        VMObject managed_pointer = m_stack[--m_stack_ptr];
        VMObject native_pointer = m_stack[--m_stack_ptr];



        std::string str = "";
        for (auto i = 0; i < MemMgrInstance().GetArrayLength(managed_pointer); ++i) {
          char c;
          MemMgrInstance().ReadFromArrayIndex(managed_pointer, i, &c);
          str += c;
        }

        std::string text = std::string("Invoked native function, name stored at managed heap address ") 
          + std::to_string(managed_pointer.as_managed_pointer()) + " (value " + str + ") with first param " + first_param.to_string()
          + " and second param " + second_param.to_string() + " and native pointer " + native_pointer.to_string();

  

        LoggerManager::GetLog(VM_LOG).AddLine(LogLevel::DEBUG, text);
      }
      break;

    case ByteCode::RETURN:
      if (m_frame_ptr == 0)  {
        return; // stop execution - returning from last function.
      }
      else {
        --m_frame_ptr;
      }
      break;
    default: 
    {
      std::string error = std::string("Fatal error: Unrecognized byte code: ")
        + std::to_string((uint32_t)code);

      LoggerManager::GetLog(VM_LOG).AddLine(LogLevel::ERROR, error);
      throw std::runtime_error(error);
    }
    }
  }
}

VM &VMInstance() {
  static VM vm;
  return vm;
}