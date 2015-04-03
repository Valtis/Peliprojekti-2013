#pragma once
#include "Component/Component.h"
#include "Message/MessageProcessor.h"
#include "VM/VMState.h"
#include <list>
#include <map>
enum class EntityType : int { BULLET, HEALTH_PICKUP };

class Entity : public MessageProcessor
{

public:
  Entity();
  ~Entity();
  
  void AddComponent(ComponentType type, std::unique_ptr<Component> c);
  void AddScript(std::unique_ptr<Component> script);
  void AddVmScript(const VMState &state);

  Component * GetComponent(ComponentType type)
  {
    if (m_components.count(type) == 0)
    {
      return nullptr;
    }
    return m_components[type].get();
  }

  void Update(double ticksPassed);

  
private:
  std::list<VMState> m_vmScripts; // list due to the fact that pointers\iterators into the list must not be invalidated on reallocation
  std::vector<std::unique_ptr<Component>> m_scripts;
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
};
