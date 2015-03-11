#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>
enum class ObjectType : char { UNINITIALIZED, INT, FLOAT, DOUBLE, BOOL };
class VMObject {
public:

  VMObject() { m_type = ObjectType::UNINITIALIZED; m_value.double_value = 0; }
  VMObject(const int32_t v) { set_int(v); }
  VMObject(const float v) { set_float(v); }
  VMObject(const double v) { set_double(v); }
  VMObject(const bool v) { set_bool(v); }

  void set_int(const int32_t v) {
    m_type = ObjectType::INT;
    m_value.int_value = v;
  }

  void set_float(const float v) {
    m_type = ObjectType::FLOAT;
    m_value.float_value = v;
  }

  void set_double(const double v) {
    m_type = ObjectType::DOUBLE;
    m_value.double_value = v;
  }

  void set_bool(const bool v) {
    m_type = ObjectType::BOOL;
    m_value.bool_value = v;
  }

  int32_t as_int() const {
    assert_type(ObjectType::INT);
    return m_value.int_value;
  }

  float as_float() const  {
    assert_type(ObjectType::FLOAT);
    return m_value.float_value;
  }

  double as_double() const  {
    assert_type(ObjectType::DOUBLE);
    return m_value.double_value;
  }

  bool as_bool() const  {
    assert_type(ObjectType::BOOL);
    return m_value.bool_value;
  }

  std::string to_string() const {
    std::string str;
    switch (m_type) {
      case ObjectType::INT:
        str = "Integer: " + std::to_string(m_value.int_value);
        break;
      case ObjectType::FLOAT:
        str = "Float: " + std::to_string(m_value.float_value);
        break;
      case ObjectType::DOUBLE:
        str = "Double: " + std::to_string(m_value.double_value);
        break;
      case ObjectType::BOOL:
        str = "Bool: " + std::to_string(m_value.bool_value);
        break;
      default:
        str = "Unknown type";
    }
    return str;
  }

private:
  void assert_type(const ObjectType t) const {
    if (m_type != t) {
      throw std::runtime_error("Invalid type");
    }
  }

  ObjectType m_type;

  union {
    int32_t int_value;
    float float_value;
    double double_value;
    bool bool_value;
  } m_value;
};