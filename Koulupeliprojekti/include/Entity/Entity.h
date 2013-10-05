#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include "Entity/EntityParent.h"


#include "Component/Component.h"
#include "Message/Message.h"

class Entity : public EntityParent
{

public:

private:

	std::unordered_map<MessageType, std::vector<MessageCallback>> m_messageHandlers;
	std::unordered_map<ComponentType, std::unique_ptr<Component>> m_components;
	std::vector<Entity> m_childs;
	EntityParent *m_parent;
};