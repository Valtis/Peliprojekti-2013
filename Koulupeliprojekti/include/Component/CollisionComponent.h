#pragma once
#include "Component/Component.h"

class CollisionComponent : public Component
{
public:
  CollisionComponent();
  ~CollisionComponent();
  void OnAttatchingToEntity() override;
  void Update(double ticksPassed) override;

private:
};