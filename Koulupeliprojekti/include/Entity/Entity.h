#pragma once
#include <map>

enum class ComponentType;
#include "Message/MessageProcessor.h"
#include "Component/Component.h"
enum class EntityType : int { BULLET, HEALTH_PICKUP };

class Entity : public MessageProcessor
{

public:
  Entity();
  ~Entity();
  
  void AddComponent(ComponentType type, std::unique_ptr<Component> c);
  void AddScript(std::unique_ptr<Component> script);
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
  std::vector<std::unique_ptr<Component>> m_scripts;
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
};
