#pragma once

enum class ComponentType : int { NONE, GRAPHICS, COLLISION, INPUT, LOCATION, VELOCITY, ACCELERATION };

class Component
{
public:
  virtual ~Component();
  virtual void Update(double ticksPassed);
protected:
  Component();

private:


};