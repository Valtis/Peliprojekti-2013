#pragma once
#include <vector>
#include <memory>

#include "Message/MessageProcessor.h"

class Entity;
class Window;

class Level : public MessageProcessor
{
public:
  Level();
  ~Level();

  void Update(double ticksPassed);

  void HandlePendingDeletions();

  const std::vector<std::unique_ptr<Entity>> &GetEntities();
  void AddEntity(std::unique_ptr<Entity> e);

private:

  MessageHandling HandleEntitySpawning(Message *msg);
  MessageHandling HandleEntityTermination(Message *msg);
  std::vector<std::unique_ptr<Entity>> m_entities;
  std::vector<std::unique_ptr<Entity>> m_ground; // potentially more than ground, rename if needed
  std::vector<Entity *> m_deletionList;

};