#pragma once
#include <map>

#include "Component/Component.h"
#include "Message/MessageProcessor.h"

enum class EntityType : int { BULLET };

class Entity : public MessageProcessor
{

public:
  Entity();
  ~Entity();
  
  void AddComponent(ComponentType type, std::unique_ptr<Component> c);
  void AddScript(std::unique_ptr<Component> script);
  Component *GetComponent(ComponentType type);
  void Update(double ticksPassed);

  
private:
  std::vector<std::unique_ptr<Component>> m_scripts;
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
};
