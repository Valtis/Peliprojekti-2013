#include "VM/VMFrame.h"

VMFrame::VMFrame() : VMFrame(nullptr) { }

VMFrame::VMFrame(VMFunction *function) : m_function(function), m_current_instruction(0) {

}

ByteCode VMFrame::GetNextInstruction() {
  return m_function->GetByteCode(m_current_instruction++);
}

void VMFrame::SetNextInstruction(uint32_t instruction) {
  m_current_instruction = instruction;
}