#pragma once

enum class ComponentType : int { NONE, GRAPHICS, COLLISION, INPUT, LOCATION, VELOCITY, ACCELERATION };

class Component
{
public:
  virtual ~Component();
protected:
  Component();

private:


};