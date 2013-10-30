#pragma once
#include "Component.h"
enum class Direction : int { LEFT, RIGHT };

class LocationComponent : public Component
{
public:
  LocationComponent();
  ~LocationComponent();

  double GetX() { return m_x; }
  double GetY() { return m_y; }
  void SetLocation(double x, double y) { m_x = x; m_y = y;}
  Direction GetDirection() { m_direction; }
protected:
  void OnAttatchingToEntity() override;

private:

  bool HandleLocationChangeMessage(Message *message);
  bool HandleCollisionMessage(Message *message);

  double m_x;
  double m_y;
  Direction m_direction;
};