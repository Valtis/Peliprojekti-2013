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
  void Update(double ticksPassed);

  void HandlePendingDeletions();

  const std::vector<std::unique_ptr<Entity>> &GetEntities();
  void AddEntity(std::unique_ptr<Entity> e);
private:

  bool HandleEntitySpawning(Message *msg);
  bool HandleEntityTermination(Message *msg);
  std::vector<std::unique_ptr<Entity>> m_entities;
  std::vector<std::unique_ptr<Entity>> m_ground; // potentially more than ground, rename if needed
  MessageProcessor m_messageProcessor;
  std::vector<Entity *> m_deletionList;

};