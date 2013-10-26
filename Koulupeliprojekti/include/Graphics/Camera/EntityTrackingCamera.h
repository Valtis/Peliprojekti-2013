#pragma once
#include "Graphics/Camera/Camera.h"
class Entity;
class EntityTrackingCamera : public Camera
{
public:
  EntityTrackingCamera();
  ~EntityTrackingCamera();
  void SetEntity(Entity *e);
  int GetX() override;
  int GetY() override;
private:
  Entity *m_entity;
};

