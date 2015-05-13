#pragma once
#include <vector>
class Entity;
class Message;
namespace ScriptMessageInterface {
  void SendBlinkingMessage(Entity *entity, int duration);
  void SendSpawnMessage(Entity *entity, int entityType);
  void SendTakeDamageMessage(Entity *entity);

  std::vector<Entity* > CollisionMessageGetFactions(Message *message);
  int GetFaction(Entity *);
};