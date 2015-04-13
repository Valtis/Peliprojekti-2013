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
  const std::vector<std::unique_ptr<Entity>> &GetStaticEntities();
  const std::vector<std::unique_ptr<Entity>> &GetStaticCollidables();

  void AddEntity(std::unique_ptr<Entity> e);
  void AddStaticEntity(std::unique_ptr<Entity> e);

  void SetLevelStartPoint(std::pair<double, double> point) { m_levelStartPoint = point; }

private:

  std::pair<double, double> m_levelStartPoint;

  MessageHandling HandleEntitySpawning(Message *msg);
  MessageHandling HandleEntityTermination(Message *msg);
  MessageHandling HandleEntityPositionReset(Message *msg);

  std::vector<std::unique_ptr<Entity>> m_entities;
  std::vector<std::unique_ptr<Entity>> m_staticEntities; // potentially more than ground, rename if needed
  std::vector<std::unique_ptr<Entity>> m_staticCollidables;
  std::vector<Entity *> m_deletionList;

};
