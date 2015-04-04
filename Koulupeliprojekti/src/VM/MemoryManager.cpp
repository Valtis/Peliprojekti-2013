#include "VM/MemoryManager.h"
#include "Utility/LoggerManager.h"
#include <string>
#ifdef _MSC_VER
// for performance counter
#include <windows.h>
#undef ERROR // windows.h really likes to define random stuff
#endif 

// 0 is reserved for null pointers and as such is invalid address. Skip first ALIGN bytes as a result
const uint32_t HEAP_BEGIN_ADDRESS = ALIGN;
const uint32_t TYPE_POINTER_SIZE = 4;
const uint32_t FORWARD_POINTER_SIZE = 4;
const uint32_t ARRAY_LENGTH_FIELD_SIZE = 4;
const uint32_t VM_NULLPTR = 0;

uint32_t ArrayMetaDataSize() {
  return TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
}

uint32_t AlignSize(uint32_t size) {
  return size + ((ALIGN - size % ALIGN) % ALIGN); // ensure alignment
}

MemoryManager::MemoryManager(uint32_t heap_size) : m_heapSize(heap_size), m_freeSpacePointer(HEAP_BEGIN_ADDRESS), m_provider(nullptr) {
  m_memory = new uint8_t[heap_size];
  m_toSpace = new uint8_t[heap_size];
}

MemoryManager::MemoryManager(MemoryManager &&other) 
  : m_heapSize(other.m_heapSize), m_memory(other.m_memory), m_toSpace(other.m_toSpace), 
    m_freeSpacePointer(other.m_freeSpacePointer) {
  other.m_memory = nullptr;
  other.m_toSpace = nullptr;
}

MemoryManager &MemoryManager::operator=(MemoryManager &&rhs) {
  if (&rhs != this) {
    m_heapSize = rhs.m_heapSize;
    m_memory = rhs.m_memory;
    m_toSpace = rhs.m_toSpace;
    m_freeSpacePointer = rhs.m_freeSpacePointer;
    rhs.m_memory = nullptr;
    rhs.m_toSpace = nullptr;
  }

  return *this;
}

MemoryManager::~MemoryManager() {
  delete[] m_memory;
  delete[] m_toSpace;
  // m_provider is a non-owning pointer and MUST NOT BE DELETED HERE
}


// array layout: 
// |1 bit          |  31 bits   | 32 bits         | 32 bits      | rest       |
// |array mark bit | array type | forward pointer | array length | array data |
// so 12 byte header containing bookkeeping information and length * sizeof(array type) bytes for array itself
VMValue MemoryManager::AllocateArray(const ValueType objectType, const uint32_t length) {
  auto requiredSpace = length*TypeSize(objectType) + ArrayMetaDataSize();

  requiredSpace = AlignSize(requiredSpace);
  
  EnsureFreeMemory(requiredSpace);

  // zero-initialization of memory before handing it over
  memset(m_memory + m_freeSpacePointer, 0, requiredSpace);

  VMValue object;
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

void MemoryManager::WriteToArrayIndex(const VMValue object, const void *value, 
    const uint32_t index, const uint32_t length) {
  auto arrayData = ArrayReadWriteCommon(object, index, length);
  memcpy(arrayData.data + index * TypeSize(arrayData.type), value, TypeSize(arrayData.type)*length);
}

void MemoryManager::ReadFromArrayIndex(const VMValue object, void *value, 
    const uint32_t index, const uint32_t length) const {
  auto arrayData = ArrayReadWriteCommon(object, index, length);
  memcpy(value, arrayData.data + index * TypeSize(arrayData.type), TypeSize(arrayData.type)*length);
}

MemoryManager::ArrayReadWriteData MemoryManager::ArrayReadWriteCommon(const VMValue object, 
    const uint32_t index, const uint32_t readWriteLength) const {
  EnsureNotNull(object);

  
  uint32_t typeField = GetTypeField(object, m_memory);
  EnsureArray(typeField);

  auto address = object.as_managed_pointer();
  ValueType type = GetArrayValueType(typeField);

  auto arrayLength = GetArrayLengthUnchecked(object, m_memory);

  if (index + readWriteLength > arrayLength || index < 0 || arrayLength == 0) {
    std::string err = "Out of bounds array access: Array length: " + std::to_string(arrayLength) + "   Index: "
      + std::to_string(index) + "   Access length: " + std::to_string(readWriteLength);
    throw std::runtime_error(err);
  }

  uint8_t *data = m_memory + address + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE + ARRAY_LENGTH_FIELD_SIZE;
  return { data, type };
}

ValueType MemoryManager::GetArrayValueType(uint32_t typeField) const {
  return static_cast<ValueType>(typeField & ~(1 << 31));
}

void MemoryManager::EnsureNotNull(VMValue object) const {
  if (object.as_managed_pointer() == VM_NULLPTR) {
    throw std::runtime_error("Null pointer");
  }
}

uint32_t MemoryManager::GetTypeField(VMValue object, uint8_t *memoryArea) const {
  uint32_t *typeField = (uint32_t *)(memoryArea + object.as_managed_pointer());
  return *typeField;
}

void MemoryManager::EnsureArray(uint32_t typeField) const {
  if (IsArray(typeField) == false) {
    throw std::runtime_error("Non-array object provided when array object expected");
  }
}

bool MemoryManager::IsArray(uint32_t typeField) const {
  return (typeField & (1 << 31)) != 0;
}

void MemoryManager::EnsureValidAccess(VMValue pointer) const{
  if (pointer.as_managed_pointer() >= m_freeSpacePointer) {
    throw std::runtime_error("Read beyond allocated memory");
  }
}

uint32_t MemoryManager::GetArrayLength(VMValue object) const {

  uint32_t typeField = GetTypeField(object, m_memory);
  EnsureNotNull(object);
  EnsureArray(typeField);
  EnsureValidAccess(object);
  return GetArrayLengthUnchecked(object, m_memory);
}

uint32_t MemoryManager::GetArrayLengthUnchecked(VMValue object, uint8_t *memoryArea) const {
  uint32_t *length_field = (uint32_t *)(memoryArea + object.as_managed_pointer()
    + TYPE_POINTER_SIZE + FORWARD_POINTER_SIZE);
  return *length_field;
}



void MemoryManager::RunGc() {
  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO,
    "Initiating GC with " + std::to_string(m_freeSpacePointer) + " bytes in use");

#ifdef _MSC_VER
  LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
  LARGE_INTEGER Frequency;

  QueryPerformanceFrequency(&Frequency);
  QueryPerformanceCounter(&StartingTime);
#endif
  Scavenge();
#ifdef _MSC_VER   
  QueryPerformanceCounter(&EndingTime);
  ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;



  ElapsedMicroseconds.QuadPart *= 1000000;
  ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

#endif

  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO,
    "GC finished. " + std::to_string(m_freeSpacePointer) + " bytes in use."
#ifdef _MSC_VER 
    + " Time spent (microseconds) : " + std::to_string(ElapsedMicroseconds.QuadPart)
#endif
    );

}

void MemoryManager::Scavenge() {
  if (m_provider == nullptr) {
    return;
  }
  /*
  std::string msg = "Memory layout dump:\n";
  for (int i = HEAP_BEGIN_ADDRESS; i < m_freeSpacePointer;) {
    VMValue f;
    f.set_managed_pointer(i);
    auto typeField = GetTypeField(f, m_memory);
    if (IsArray(typeField)) {
      msg += "\nArray at address " + std::to_string(i) + "\n";
      msg += "Type tag " + TypeToString(GetArrayValueType(typeField)) + " (Size: " + std::to_string(TypeSize(GetArrayValueType(typeField))) + ")\n";
      msg += "Length: " + std::to_string(GetArrayLengthUnchecked(f, m_memory)) + "\n";

    }
    else {
      msg += "\nInvalid object in memory dump - terminating dump\n";
      break;
    }

    i += CalculateObjectSize(f, m_memory);
  }


  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::DEBUG,msg);*/


  std::vector<VMValue *> rootSet = m_provider->GetRootSet();

  LoggerManager::GetLog(MEMORY_LOG).AddLine(LogLevel::INFO, "Root set size: " + std::to_string(rootSet.size()) + " objects");

  EvacuateRootSet(rootSet);
  EvacuateObjects();
  std::swap(m_toSpace, m_memory);
}

void MemoryManager::EvacuateRootSet(std::vector<VMValue *> rootSet) {
  m_freeSpacePointer = HEAP_BEGIN_ADDRESS;
  for (auto &value : rootSet) {
    MoveObject(value);
  }
}

void MemoryManager::EvacuateObjects() {
  auto scanPointer = HEAP_BEGIN_ADDRESS;

  while (scanPointer < m_freeSpacePointer) {
    VMValue pointer;
    pointer.set_managed_pointer(scanPointer);
    CopyPointedObjects(pointer);
    scanPointer += CalculateObjectSize(pointer, m_toSpace);
  }
}

void MemoryManager::CopyPointedObjects(VMValue &pointer) {

  auto typeField = GetTypeField(pointer, m_toSpace);

  if (IsArray(typeField)) {
    if (GetArrayValueType(typeField) != ValueType::MANAGED_POINTER) {
      return;
    }

    auto position = pointer.as_managed_pointer() + ArrayMetaDataSize();
    auto length = GetArrayLengthUnchecked(pointer, m_memory);
    auto typeSize = TypeSize(GetArrayValueType(typeField));
    for (size_t i = 0; i < length; ++i) {
      uint32_t pointer;
      auto pointerInArrayLocation = position + i*typeSize;
      memcpy(&pointer, m_memory + pointerInArrayLocation, sizeof(pointer));
      
      VMValue tempPointer;
      tempPointer.set_managed_pointer(pointer);
      MoveObject(&tempPointer);

      pointer = tempPointer.as_managed_pointer();
      memcpy(m_memory + pointerInArrayLocation, &pointer, sizeof(pointer));
    }
  }
  else {
    throw std::logic_error("Copying not implemented for non-arrays");
  }
}

// returns size of the object moved
void MemoryManager::MoveObject(VMValue *pointer) {

  auto forwardingPointer = GetForwardingPointer(pointer);
  if (forwardingPointer != 0) {
    UpdatePointer(pointer, forwardingPointer);
    return;
  }

  PerformCopy(pointer);  
}

void MemoryManager::PerformCopy(VMValue *pointer) {

  auto size = CalculateObjectSize(*pointer, m_memory);
  memcpy(m_toSpace + m_freeSpacePointer, m_memory + pointer->as_managed_pointer(), size);

  if (IsArray(GetTypeField(*pointer, m_memory))) {
    VMValue foobar;
    foobar.set_managed_pointer(m_freeSpacePointer);

  }
  

  UpdateForwardingPointer(pointer);

  UpdatePointer(pointer, m_freeSpacePointer);
  m_freeSpacePointer += size;
}

void MemoryManager::UpdateForwardingPointer(VMValue *pointer) {
  auto position = pointer->as_managed_pointer() + TYPE_POINTER_SIZE; // skip first header field
  memcpy(m_memory + position, &m_freeSpacePointer, sizeof(m_freeSpacePointer));
}


uint32_t MemoryManager::GetForwardingPointer(VMValue *pointer) {
  uint32_t ptr;
  auto position = pointer->as_managed_pointer() + TYPE_POINTER_SIZE; // skip first header field
  memcpy(&ptr, m_memory + position, sizeof(FORWARD_POINTER_SIZE));
  return ptr;
}

void MemoryManager::UpdatePointer(VMValue *pointer, uint32_t newValue) {
  VMValue newLocation;
  newLocation.set_managed_pointer(newValue);
  *pointer = newLocation;
}
uint32_t MemoryManager::CalculateObjectSize(const VMValue pointer, uint8_t *memoryArea) const {
  auto typeField = GetTypeField(pointer, memoryArea);

  if (IsArray(typeField)) {
    return AlignSize(GetArrayLengthUnchecked(pointer, memoryArea)*TypeSize(GetArrayValueType(typeField)) + ArrayMetaDataSize());
  } else {
    throw std::logic_error("Size calculation not implemented for non-arrays");
  }
}

MemoryManager &MemMgrInstance() {
  // should probably read the values from ini or something; hard coded for now
  static MemoryManager manager(1024 * 1024 * 32);
  return manager;
}