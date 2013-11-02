#pragma once
#include <map>

#include "Entity/EntityParent.h"
#include "Component/Component.h"
#include "Message/MessageProcessor.h"

enum class EntityType : int { BULLET };

class Entity : public EntityParent
{

public:
  Entity();
  ~Entity();
  void SendMessage(Message *message) override;
  void RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback) override;
  void SendMessageToParent(Message *message); 
  void SendMessageToChildren(Message *message);
  void AddComponent(ComponentType type, std::unique_ptr<Component> c);
  void AddScript(std::unique_ptr<Component> script);
  Component *GetComponent(ComponentType type);
  void Update(double ticksPassed);

  void SetParent(EntityParent *parent) { m_parent = parent; }
  
private:
  std::vector<std::unique_ptr<Component>> m_scripts;
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
  MessageProcessor m_messageProcessor;
  std::vector<Entity *> m_children;
  EntityParent *m_parent;
};
