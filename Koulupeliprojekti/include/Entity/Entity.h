#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Entity/EntityParent.h"


#include "Component/Component.h"
#include "Message/Message.h"

class Entity : public EntityParent
{

public:

private:

  std::map<MessageType, std::vector<MessageCallback>> m_messageHandlers;
  std::map<ComponentType, std::unique_ptr<Component>> m_components;
  std::vector<Entity> m_childs;
  EntityParent *m_parent;
};