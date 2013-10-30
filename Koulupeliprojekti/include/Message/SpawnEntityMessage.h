#pragma once
#include "Message.h"

class Entity;
enum class EntityType;

class SpawnEntityMessage : public Message
{
public:
  SpawnEntityMessage(EntityType type, Entity *spawner) : m_entityType(type), m_spawner(spawner) { }
  ~SpawnEntityMessage() { }
  MessageType GetType() const override { return MessageType::SPAWN_ENTITY; }
  EntityType GetEntityType() const { return m_entityType; }
  const Entity *Spawner() const { return m_spawner; }
private:
  const EntityType m_entityType;
  const Entity *m_spawner;
};