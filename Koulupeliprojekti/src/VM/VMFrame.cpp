#include "VM/VMFrame.h"

VMFrame::VMFrame() : m_function(nullptr) { }

VMFrame::VMFrame(VMFunction *function) : m_function(function) {

}

ByteCode VMFrame::GetNextInstruction() {
  return m_function->GetByteCode(m_current_instruction++);
}

void VMFrame::SetNextInstruction(uint32_t instruction) {
  m_current_instruction = instruction;
}