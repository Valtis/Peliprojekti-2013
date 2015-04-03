#include "VM/VM.h"
#include "VM/VMState.h"
#include "Utility/LoggerManager.h"
#include "VM/MemoryManager.h"
#include "VM/FFI/ConversionFunctions.h"
#include "VM/FFI/NativeBinding.h"
#include "VM/VMOperations.h"

#include <map>

const uint32_t stackSize = 4096;
const uint32_t frameSize = 1024;
VM::VM() {

  m_stack.reserve(stackSize);
  m_frames.reserve(frameSize);
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

  return ReturnValue();

}

void VM::InitializeVMForExecution(const std::string & functionName, std::vector<VMValue> objects, const VMFunction *function)
{
  m_stack.clear();
  m_frames.clear();

  m_stack.reserve(stackSize);
  m_frames.reserve(frameSize);

  m_frames.push_back(VMFrame{ function });

  for (const auto &o : objects) {
    m_stack.push_back(o);
  }
}


void VM::Execute(VMState &state) {

  for (;;)  {
    auto code = m_frames.back().GetNextInstruction();

    switch (code) {
    case ByteCode::POP:
      Op::PopValue(m_stack);
    return;

    case ByteCode::LOAD_LOCAL:
      Op::LoadLocal(state, m_stack, m_frames);
      break;
    case ByteCode::STORE_LOCAL:
      Op::StoreLocal(state, m_stack, m_frames);
      break;
    case ByteCode::PUSH_INTEGER:
      Op::PushInteger(m_stack, m_frames);
      break; 
    case ByteCode::LOAD_STATIC_OBJECT: 
      Op::LoadStaticObject(state, m_stack, m_frames);
      break;
    case ByteCode::STORE_STATIC_OBJECT:
      Op::StoreStaticObject(state, m_stack, m_frames);
      break;
    case ByteCode::JUMP_IF_ZERO:
      Op::JumpIfZero(state, m_stack, m_frames);
      break;
    case ByteCode::JUMP_IF_NEGATIVE:
      Op::JumpIfNegative(state, m_stack, m_frames);
      break; 
    case ByteCode::JUMP_IF_POSITIVE:
      Op::JumpIfPositive(state, m_stack, m_frames);
      break;
    case ByteCode::ADD_INTEGER:
      Op::AddInteger(m_stack);
      break;
    
    case ByteCode::SUB_INTEGER:
      Op::SubInteger(m_stack);
      break;    
    case ByteCode::MUL_INTEGER:
      Op::MulInteger(m_stack);
      break;
    case ByteCode::DIV_INTEGER:
      Op::DivInteger(m_stack);
      break;

    case ByteCode::INVOKE_NATIVE:
      Op::InvokeNative(state, m_stack);
      break;
    case ByteCode::INVOKE_MANAGED:
      Op::InvokeManaged(state, m_stack, m_frames);
      break;

    case ByteCode::ALLOCATE_INTEGER_ARRAY:
      Op::AllocateIntegerArray(m_stack);
      break;
    case ByteCode::DOUBLE_TO_INTEGER:
      Op::DoubleToInteger(m_stack);

    case ByteCode::RETURN:
      if (!Op::Return(m_frames)) {
        return;
      }
      break;
    
    case ByteCode::NOP:
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

VMValue VM::ReturnValue() {
  // return topmost stack item, if any
  if (!m_stack.empty()) {
    return m_stack.back();
  }
  else {
    return{};
  }
}

void VM::BuildStackTraceAndThrow(const std::exception &ex) {

  auto &log = LoggerManager::GetLog(VM_LOG);
  std::string error = std::string("An error has occurred during script execution: ") + ex.what();
  std::string stack_trace = "\n" + error + "\n";
 
  stack_trace += CreateStackTrace();


  log.AddLine(LogLevel::ERROR, stack_trace);

  throw std::runtime_error(error);
}

std::string VM::CreateStackTrace() {
  std::string stackTrace;
  AddBasicScriptInfoToErrorMessage(stackTrace);
  AddFrameStackToErrorMessage(stackTrace);
  AddValueStackToErrorMessage(stackTrace);
  return stackTrace;
}

void VM::AddBasicScriptInfoToErrorMessage(std::string &stack_trace) {
  auto &frame = m_frames.back();
  stack_trace += "\tWhen executing script function '" + frame.GetFunctionName() + "'\n";
  stack_trace += "\tWhen executing instruction '" + std::string(GetByteCodeName(frame.GetPreviousInstruction())) + "'\n";
  stack_trace += "\tWith program counter value " + std::to_string(frame.GetProgramCounter()) + "\n\n";
}

void VM::AddFrameStackToErrorMessage(std::string &stack_trace) {
  for (int i = m_frames.size() - 2; i >= 0; --i) {
    stack_trace += "\t\tCalled from script function '" + m_frames[i].GetFunctionName() + "'\n";
    stack_trace += "\t\tWith program counter value " + std::to_string(m_frames[i].GetProgramCounter()) + "\n\n";
  }
}

void VM::AddValueStackToErrorMessage(std::string &stack_trace) {
  stack_trace += "Script stack:\n\n";
  for (int i = m_stack.size() - 1; i >= 0; --i) {
    stack_trace += std::to_string(i) + ": " + m_stack[i].to_string() + "\n";
  }
}


VM &VMInstance() {
  static VM vm;
  return vm;
}