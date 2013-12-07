#pragma once
#include "Component.h"
#include <map>
#include "Utility/DirectionEnum.h"

class LocationComponent : public Component
{
public:
  LocationComponent();
  LocationComponent(double x, double y);
  ~LocationComponent();
  void Update(double ticksPassed) override;
  double GetX() { return m_x; }
  double GetY() { return m_y; }
  bool CanIJump();
  void SetLocation(double x, double y) { m_x = x; m_y = y;}
  Direction GetDirection() { return m_direction; }
  Direction GetFiringDirection() { return m_firingDirection; }
  void SetDirection(Direction d) { m_direction = d; }
protected:
  void OnAttatchingToEntity() override;

private:
  MessageHandling HandleSetFiringDirectionMessage(Message *message);
  MessageHandling HandleLocationChangeMessage(Message *message);
  MessageHandling HandleCollisionMessage(Message *message);
  MessageHandling HandleSetLocationMessage(Message *message);

  double m_x;
  double m_y;

  Direction m_firingDirection;
  Direction m_direction;
  std::map<CollisionSide, bool> m_collision;
};
