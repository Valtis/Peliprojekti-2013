#pragma once
#include "VM/RootSetProvider.h"
#include "VM/VMValue.h"
#include <vector>


#define ALIGN 4 // memory allocation aligned to multiples of 4. Allocations are padded as needed

// visual studio 2013 and earlier do not support alignas
#if _MSC_VER <= 1800
#define alignas(x) __declspec(align(x))
#endif

class MemoryManager {
public:
  MemoryManager(uint32_t heap_size);
  MemoryManager(const MemoryManager &) = delete;
  MemoryManager(MemoryManager &&);
  MemoryManager &operator=(const MemoryManager &) = delete;
  MemoryManager &operator=(MemoryManager &&);
  ~MemoryManager();

  VMValue AllocateArray(const ValueType objectType, const uint32_t length);
  void WriteToArrayIndex(const VMValue object, const void *value, const uint32_t index, const uint32_t length);
  void ReadFromArrayIndex(const VMValue object, void *value, const uint32_t index, const uint32_t length) const;

  void RunGc();

  uint32_t GetArrayLength(VMValue object) const;

  void SetRootSetProvider(RootSetProvider *provider) { m_provider = provider;  }
private:

  void EnsureFreeMemory(uint32_t requiredSpace);

  struct ArrayReadWriteData {
    uint8_t *data;
    ValueType type;
  };

  ArrayReadWriteData ArrayReadWriteCommon(const VMValue object, const uint32_t index, const uint32_t length) const;

  uint32_t GetArrayLengthUnchecked(VMValue obj, uint8_t *memoryArea) const;
  uint32_t GetTypeField(VMValue object, uint8_t *memoryArea) const;
  ValueType GetArrayValueType(uint32_t typeField) const;

  void EnsureNotNull(VMValue object) const;
  void EnsureArray(uint32_t type) const;
  bool IsArray(uint32_t typeField) const;
  void EnsureValidAccess(VMValue pointer) const;

  void Scavenge();
  void EvacuateRootSet(std::vector<VMValue *> rootSet);
  void EvacuateObjects();
  void CopyPointedObjects(VMValue &pointer);
  void  MoveObject(VMValue *pointer); 
  void PerformCopy(VMValue *pointer);

  void UpdatePointer(VMValue *pointer, uint32_t newLocation);
  void UpdateForwardingPointer(VMValue *pointer);
  uint32_t GetForwardingPointer(VMValue *pointer);

  uint32_t CalculateObjectSize(const VMValue pointer, uint8_t *memoryArea) const;
  
  alignas(ALIGN) uint8_t *m_memory;
  alignas(ALIGN) uint8_t *m_toSpace; // reserved for garbage collection

  uint32_t m_heapSize;
  uint32_t m_freeSpacePointer;
  RootSetProvider *m_provider; // non-owning pointer. MUST NOT BE DELETED
};


MemoryManager &MemMgrInstance();