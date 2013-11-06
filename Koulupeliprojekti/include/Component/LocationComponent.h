#pragma once
#include "Component.h"
enum class Direction : int { LEFT, RIGHT };

class LocationComponent : public Component
{
public:
  LocationComponent();
  ~LocationComponent();
  void Update(double ticksPassed) { m_collision = false; }
  double GetX() { return m_x; }
  double GetY() { return m_y; }
  bool CanIJump() { return m_collision; }
  void SetLocation(double x, double y) { m_x = x; m_y = y;}
  Direction GetDirection() { return m_direction; }
  void SetDirection(Direction d) { m_direction = d; }
protected:
  void OnAttatchingToEntity() override;

private:

  bool HandleLocationChangeMessage(Message *message);
  bool HandleCollisionMessage(Message *message);

  double m_x;
  double m_y;
  Direction m_direction;
  bool m_collision;
};
