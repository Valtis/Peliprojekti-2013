#pragma once
#include <map>

#include "Entity/EntityParent.h"
#include "Component/Component.h"
#include "Message/MessageProcessor.h"
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
  Component *GetComponent(ComponentType type);
  void Update(double ticksPassed);
  
private:
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
  MessageProcessor m_messageProcessor;
  std::vector<Entity *> m_children;
  EntityParent *m_parent;
};
