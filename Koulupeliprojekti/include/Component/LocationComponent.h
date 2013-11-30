#pragma once
#include "Component.h"
#include <map>
enum class Direction : int { LEFT, RIGHT, UP, DOWN };

class LocationComponent : public Component
{
public:
  LocationComponent();
  ~LocationComponent();
  void Update(double ticksPassed) override;
  double GetX() { return m_x; }
  double GetY() { return m_y; }
  bool CanIJump();
  void SetLocation(double x, double y) { m_x = x; m_y = y;}
  Direction GetDirection() { return m_direction; }
  void SetDirection(Direction d) { m_direction = d; }
protected:
  void OnAttatchingToEntity() override;

private:

  MessageHandling HandleLocationChangeMessage(Message *message);
  MessageHandling HandleCollisionMessage(Message *message);
  MessageHandling HandleSetLocationMessage(Message *message);

  double m_x;
  double m_y;
  Direction m_direction;
  std::map<CollisionSide, bool> m_collision;
};
