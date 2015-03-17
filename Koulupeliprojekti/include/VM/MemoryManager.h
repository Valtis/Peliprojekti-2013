#pragma once
#include "VMObject.h"

class MemoryManager {
public:
  MemoryManager(uint32_t heap_size);
  VMObject AllocateArray(const ObjectType objectType, const uint32_t length);
  void WriteToArrayIndex(const VMObject object, const uint32_t index, const void *value);
  void ReadFromArrayIndex(const VMObject object, const uint32_t index, void *value);

  void RunGc();

  uint32_t GetArrayLength(VMObject object);
private:

  void EnsureFreeMemory(uint32_t requiredSpace);
  uint32_t GetArrayLengthUnchecked(VMObject obj);
  uint32_t GetTypeField(VMObject object);
  void EnsureArray(uint32_t type);

  uint8_t *m_memory;
  uint8_t *m_toSpace; // reseved for garbage collection

  uint32_t m_heapSize;
  uint32_t m_freeSpacePointer;
};


MemoryManager &MemMgrInstance();