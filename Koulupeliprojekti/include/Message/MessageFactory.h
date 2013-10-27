#pragma once
class Message;
#include <memory>
#include <SDL.h>
#include "Entity/Entity.h"

namespace MessageFactory
{
  std::unique_ptr<Message> CreateLocationChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateCollisionMessage(Entity *entity, SDL_Rect &intersection);
}