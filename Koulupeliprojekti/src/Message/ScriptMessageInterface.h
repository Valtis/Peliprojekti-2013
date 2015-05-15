#pragma once
#include <vector>
#include <cstdint>
class Entity;
class Message;

namespace ScriptMessageInterface {
  void SendBlinkingMessage(Entity *entity, int duration);
  void SendSpawnMessage(Entity *entity, int entityType);
  void SendAddHealthMessage(Entity *entity, int32_t amount);
  void SendTakeDamageMessage(Entity *entity, int32_t amount);
  void SendPlaySoundEffectMessage(Entity *entity, int32_t id);
  void SendVisibilityMessage(Entity *entity, bool visibility);
  void SendTerminateEntityMessage(Entity *entity); 

  std::vector<Entity* > CollisionMessageGetFactions(Message *message); 
  int32_t BlinkMessageGetDuration(Message *message);
  int32_t GetHitType(Message *message);
  int32_t GetFaction(Entity *);
}