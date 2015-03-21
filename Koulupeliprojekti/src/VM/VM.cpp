#include "VM/VM.h"
#include "VM/VMState.h"
#include "Utility/LoggerManager.h"
#include "VM/MemoryManager.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/NativeBinding.h"

#include <map>
VM::VM() {
}

// needs to be broken into smaller functions.
VMValue VM::InvokeFunction(VMState &state, const std::string &functionName, std::vector<VMValue> objects) {
  
  auto function = state.GetFunction(functionName);
  if (function == nullptr) {
    return{};
  }
 
  InitializeVMForExecution(functionName, objects, function);
  
  try {
    Execute(state);
  } catch (const std::exception &ex) {
    BuildStackTraceAndThrow(ex);
  }

  // return topmost stack item, if any
  if (m_stack_ptr != 0) {
    return m_stack[m_stack_ptr];
  } else {
    return {};
  }
}

void VM::InitializeVMForExecution(const std::string & functionName, std::vector<VMValue> objects, VMFunction *function)
{
  auto &log = LoggerManager::GetLog(VM_LOG);
  log.AddLine(LogLevel::DEBUG, "Invoking script function " + functionName);

  int i = 0;
  for (const auto &o : objects) {
    log.AddLine(LogLevel::DEBUG, "Parameter " + std::to_string(++i) + ": " + o.to_string());
  }


  m_stack_ptr = 0;
  m_frame_ptr = 0;
  m_frames[m_frame_ptr] = VMFrame{ function };

  for (const auto &o : objects) {
    m_stack[m_stack_ptr++] = o;
  }
}


void VM::Execute(VMState &state) {

  for (;;)  {
    auto code = m_frames[m_frame_ptr].GetNextInstruction();

    switch (code) {

    case ByteCode::PUSH_INTEGER:
      m_stack[m_stack_ptr++] = VMValue{ (int32_t)m_frames[m_frame_ptr].GetNextInstruction() };
      break; 
    case ByteCode::PUSH_CONSTANT_OBJECT: 
      m_stack[m_stack_ptr++] = state.GetPermanentStorageObject((int32_t)m_frames[m_frame_ptr].GetNextInstruction());
      break;

    case ByteCode::INVOKE_NATIVE:
      
      state.GetNativeBinding(ToNativeType<std::string>(m_stack[--m_stack_ptr]))
        (m_stack.data(), m_stack_ptr);
     
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

void VM::BuildStackTraceAndThrow(const std::exception &ex)
{

  auto &log = LoggerManager::GetLog(VM_LOG);
  std::string error = std::string("An error has occurred during script execution: ") + ex.what();
  std::string stack_trace = "\n" + error + "\n";
 
  AddBasicScriptInfoToErrorMessage(stack_trace);
  AddFrameStackToErrorMessage(stack_trace);
  AddValueStackToErrorMessage(stack_trace);


  log.AddLine(LogLevel::ERROR, stack_trace);

  throw std::runtime_error(error);
}

void VM::AddBasicScriptInfoToErrorMessage(std::string &stack_trace)
{
  auto &frame = m_frames[m_frame_ptr];
  stack_trace += "\tWhen executing script function '" + frame.GetFunctionName() + "'\n";
  stack_trace += "\tWhen executing instruction '" + std::string(GetByteCodeName(frame.GetPreviousInstruction())) + "'\n";
  stack_trace += "\tWith program counter value " + std::to_string(frame.GetProgramCounter()) + "\n\n";
}

void VM::AddFrameStackToErrorMessage(std::string &stack_trace)
{
  for (int i = m_frame_ptr - 1; i >= 0; --i) {
    stack_trace += "\t\tCalled from script function '" + m_frames[i].GetFunctionName() + "'\n";
    stack_trace += "\t\tWith program counter value " + std::to_string(m_frames[i].GetProgramCounter()) + "\n\n";
  }
}

void VM::AddValueStackToErrorMessage(std::string &stack_trace)
{
  stack_trace += "Script stack:\n\n";
  for (int i = m_stack_ptr - 1; i >= 0; --i) {
    stack_trace += std::to_string(i) + ": " + m_stack[i].to_string() + "\n";
  }
}


VM &VMInstance() {
  static VM vm;
  return vm;
}