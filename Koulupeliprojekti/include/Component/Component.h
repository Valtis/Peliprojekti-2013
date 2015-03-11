#pragma once

#include "Message/Message.h"
#include "Message/MessageFactory.h"
enum class ComponentType : int { NONE, GRAPHICS, COLLISION, INPUT, LOCATION, 
  VELOCITY, ACCELERATION, AI, PHYSICS, FACTION, HEALTH, SOUND };
class Entity;

class Component
{
public:
  virtual ~Component();
  virtual void Update(double ticksPassed);
  void Attach(Entity *e);
protected:
  Component();
  Entity *GetOwner() { return m_owner; }

  virtual void OnAttatchingToEntity();
private:
  Entity *m_owner;

};
