#include "VM/VMObject.h"
uint32_t TypeSize(ObjectType type) {
  switch (type) {
  case ObjectType::UNINITIALIZED:
    return 0;
  case ObjectType::INT:
    return sizeof(int32_t);
  case ObjectType::FLOAT:
    return sizeof(float);
  case ObjectType::DOUBLE:
    return sizeof(double);
  case ObjectType::BOOL:
    return sizeof(bool);
  case ObjectType::CHAR:
    return sizeof(char);
  case ObjectType::NATIVE_POINTER:
    return sizeof(void *);
  case ObjectType::MANAGED_POINTER:
    return 0;
  }
}