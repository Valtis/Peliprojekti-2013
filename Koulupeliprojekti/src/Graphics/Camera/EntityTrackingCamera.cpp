#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Component/LocationComponent.h"
#include <SDL_assert.h>
EntityTrackingCamera::EntityTrackingCamera()
{

}

EntityTrackingCamera::~EntityTrackingCamera()
{

}

void EntityTrackingCamera::SetEntity(Entity *e)
{
  m_entity = e;
}

int EntityTrackingCamera::GetX()
{
  auto locationComponent = static_cast<LocationComponent *>(m_entity->GetComponent(ComponentType::LOCATION));
  SDL_assert(locationComponent != nullptr);
  return locationComponent->GetX();
}

int EntityTrackingCamera::GetY()
{
  auto locationComponent = static_cast<LocationComponent *>(m_entity->GetComponent(ComponentType::LOCATION));
  SDL_assert(locationComponent != nullptr);
  return locationComponent->GetY();
}