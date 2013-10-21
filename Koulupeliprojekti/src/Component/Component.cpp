#include "Component/Component.h"

Component::Component()
{

}

Component::~Component()
{

}

void Component::Update(double ticksPassed)
{

};

void Component::Attach(Entity *e)
{
  m_owner = e;
  OnAttatchingToEntity();
}

void Component::OnAttatchingToEntity()
{

}