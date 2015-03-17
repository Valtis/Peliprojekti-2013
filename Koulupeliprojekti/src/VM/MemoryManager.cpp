#include "VM/MemoryManager.h"
#include "Utility/LoggerManager.h"
#include <string>

#define HEAP_BEGIN_ADDRESS 16 // 0 is reserved for null pointers and as such is invalid address. Skip first 16 bytes as a result
#define ALIGN 16 // memory allocation aligned to multiples of 16. Allocations are padded as needed
#define TYPE_POINTER_SIZE 4
#define FORWARD_POINTER_SIZE 4
#define ARRAY_LENGTH_FIELD_SIZE 4

MemoryManager::MemoryManager(uint32_t heap_size) : m_heapSize(heap_size), m_freeSpacePointer(HEAP_BEGIN_ADDRESS) {
  m_memory = new uint8_t[heap_size];
  m_toSpace = new uint8_t[heap_size];
}


// array layout: 
// |1 bit          |  31 bits   | 32 bits         | 32 bits      | rest       |
// |array mark bit | array type | forward pointer | array length | array data |
// so 12 byte header containing bookkeeping information and length * sizeof(array type) bytes for array itself
VMObject MemoryManager::AllocateArray(const ObjectType objectType, const uint32_t length) {
  auto requiredSpace = length*TypeSize(objectType) + FORWARD_POINTER_SIZE + TYPE_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  requiredSpace = requiredSpace + (ALIGN - requiredSpace % ALIGN) % ALIGN; // ensure alignment
  EnsureFreeMemory(requiredSpace);
 
  VMObject object;
  object.set_managed_pointer(m_freeSpacePointer);
  uint32_t *typeField = (uint32_t *)(m_memory + m_freeSpacePointer);
  *typeField = (1 << 31) | static_cast<uint32_t>(objectType);

  uint32_t *lengthField = (uint32_t *)(m_memory + m_freeSpacePointer + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
  *lengthField = length;

  m_freeSpacePointer += requiredSpace; 
  return object;
}

void MemoryManager::EnsureFreeMemory(uint32_t requiredSpace) {
  if (m_heapSize - m_freeSpacePointer < requiredSpace) {
    LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO, "Allocation failure due to low memory, initiating GC");
    RunGc();

    if (m_heapSize - m_freeSpacePointer < requiredSpace) {
      LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::ERROR, "Failed to allocate memory after GC. Out of memory");
      throw std::runtime_error("Out of memory!");
    }
  }
}

void MemoryManager::WriteToArrayIndex(const VMObject object, const uint32_t index, const void *value) {

  uint32_t typeField = GetTypeField(object);
  EnsureArray(typeField);

  auto address = object.as_managed_pointer();
  ObjectType type = static_cast<ObjectType>(typeField & ~(1 << 31));

  auto length = GetArrayLengthUnchecked(object);
  
  if (index >= length || index < 0) {
    throw std::runtime_error("Out of bounds array access");
  }
  
  uint8_t *data = m_memory + address + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  memcpy(data + index * TypeSize(type), value, TypeSize(type));
}

void MemoryManager::ReadFromArrayIndex(const VMObject object, const uint32_t index, void *value) {
  // check that array bit is set 

  uint32_t typeField = GetTypeField(object);
  EnsureArray(typeField);

  auto address = object.as_managed_pointer();
  ObjectType type = static_cast<ObjectType>(typeField & ~(1 << 31));

  auto length = GetArrayLengthUnchecked(object);

  if (index >= length || index < 0) {
    throw std::runtime_error("Out of bounds array access");
  }

  uint8_t *data = m_memory + address + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  memcpy(value, data + index * TypeSize(type), TypeSize(type));
}

uint32_t MemoryManager::GetTypeField(VMObject object) {
  uint32_t *typeField = (uint32_t *)(m_memory + object.as_managed_pointer());
  return *typeField;
}

void MemoryManager::EnsureArray(uint32_t typeField) {
  if (typeField & (1 << 31) == 0) {
    throw std::runtime_error("Non-array object provided when array object expected");
  }
}

uint32_t MemoryManager::GetArrayLength(VMObject object) {

  uint32_t typeField = GetTypeField(object);
  EnsureArray(typeField);
  return GetArrayLengthUnchecked(object);
}

uint32_t MemoryManager::GetArrayLengthUnchecked(VMObject object) {
  uint32_t *length_field = (uint32_t *)(m_memory + object.as_managed_pointer() + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
  return *length_field;
}


void MemoryManager::RunGc() {
  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO, 
    "Initiating GC with " + std::to_string(m_freeSpacePointer) + " bytes in use");


  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO,
    "GC finished. " + std::to_string(m_freeSpacePointer) + " bytes in use");

}




MemoryManager &MemMgrInstance() {
  // should probably read the values from ini or something; hard coded for now
  static MemoryManager manager(1024 * 1024 * 32);
  return manager;
}