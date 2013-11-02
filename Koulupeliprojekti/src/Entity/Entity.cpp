#include "Entity/Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::RegisterMessageHandler(MessageType type, Priority priority, MessageCallback callback)
{
  m_messageProcessor.RegisterMessageHandler(type, priority, callback);
}

void Entity::SendMessage(Message *message)
{
  if (!m_messageProcessor.SendMessage(message))
  {
    SendMessageToParent(message);
  }
}

void Entity::SendMessageToParent(Message *message)
{
  if (m_parent != nullptr)
  {
    m_parent->SendMessage(message);
  }
}


void Entity::SendMessageToChildren(Message *message)
{
  for (auto &child : m_children)
  {
    child->SendMessage(message);
  }
}


void Entity::AddComponent(ComponentType type, std::unique_ptr<Component> c)
{
  if (type != ComponentType::NONE)
  {
    c->Attach(this);
    m_components[type] = std::move(c);
  }
}

void Entity::AddScript(std::unique_ptr<Component> script)
{
  script->Attach(this);
  m_scripts.push_back(std::move(script));
}

Component *Entity::GetComponent(ComponentType type)
{
  if (m_components.count(type) == 0)
  {
    return nullptr;
  }

  return m_components[type].get();
}

void Entity::Update(double ticksPassed)
{
  for (auto &component : m_components)
  {
    component.second->Update(ticksPassed);
  }

  for (auto &script : m_scripts)
  {
    script->Update(ticksPassed);
  }
}