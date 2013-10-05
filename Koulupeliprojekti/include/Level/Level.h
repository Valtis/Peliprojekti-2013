#pragma once
#include <vector>
#include <memory>
#include "Entity/EntityParent.h"

class Entity;
class Level : public EntityParent
{
public:
  Level();
  ~Level();

  void AddMessage(std::unique_ptr<Message> message) override;
  void RegisterMessageHandler(MessageType type, MessageCallback callback) override; 

private:

  std::vector<std::unique_ptr<Entity>> m_Entities;

};