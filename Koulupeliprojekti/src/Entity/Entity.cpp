#include "Entity/Entity.h"
#include "VM/VM.h"
Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::AddComponent(ComponentType type, std::unique_ptr<Component> c)
{
  if (type != ComponentType::NONE)
  {
    c->Attach(this);
    m_components[type] = std::move(c);
  }
}

void Entity::AddVmScript(const VMState &state) {
  m_vmScripts.push_back(state);
  VMInstance().InvokeFunction(m_vmScripts.back(), "initialize", { &m_messageInterface, this, &m_vmScripts.back() });
}

void Entity::AddScript(std::unique_ptr<Component> script)
{
  script->Attach(this);
  m_scripts.push_back(std::move(script));
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

  for (auto &script : m_vmScripts) {
    VMInstance().InvokeFunction(script, "update", { ticksPassed });
  }

}
