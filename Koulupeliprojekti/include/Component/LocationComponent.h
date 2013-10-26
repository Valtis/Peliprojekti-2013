#pragma once
#include "Component.h"

class LocationComponent : public Component
{
public:
  LocationComponent();
  ~LocationComponent();

  double GetX() { return m_x; }
  double GetY() { return m_y; }
  void SetLocation(double x, double y) { m_x = x; m_y = y;}
  double GetRotation() { m_rotation; }
protected:
  void OnAttatchingToEntity() override;

private:

  bool HandleLocationChangeMessage(Message *message);

  double m_x;
  double m_y;
  double m_rotation;
};