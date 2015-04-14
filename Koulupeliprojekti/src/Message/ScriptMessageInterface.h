#pragma once

class Entity;

namespace ScriptMessageInterface {
  void SendBlinkingMessage(Entity *entity, int duration);
  void SendSpawnMessage(Entity *entity, int entityType);
};