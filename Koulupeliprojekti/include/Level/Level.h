#pragma once
#include <vector>
#include <memory>
#include "Entity/EntityParent.h"
#include "Message/MessageProcessor.h"
class Entity;
class Level : public EntityParent
{
public:
  Level();
  ~Level();

  void SendMessage(Message *message) override;
  void RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback) override; 
  void Update(double ticks_passed);
private:

  std::vector<std::unique_ptr<Entity>> m_Entities;
  MessageProcessor m_messageProcessor;

};