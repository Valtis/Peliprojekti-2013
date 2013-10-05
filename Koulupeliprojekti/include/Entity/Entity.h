#pragma once
#include <vector>
#include <queue>
#include <map>

#include "Entity/EntityParent.h"
#include "Component/Component.h"

class Entity : public EntityParent
{

public:
  void AddMessage(std::unique_ptr<Message> message) override;
  void RegisterMessageHandler(MessageType type, MessageCallback callback) override; 

private:
  void HandleMessages();
  void HandleSingleMessage(std::unique_ptr<Message> message);
  void SendMessageToParent(std::unique_ptr<Message> message);
  void PassMessageToHandlers( std::vector<MessageCallback> handlers, std::unique_ptr<Message> &message );

  // note: should handlers/queue be extracted to separate class (MessageQueue)
  std::map<MessageType, std::vector<MessageCallback>> m_messageHandlers;
  std::queue<std::unique_ptr<Message>> m_messages;
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
  std::vector<Entity> m_childs;
  EntityParent *m_parent;
};