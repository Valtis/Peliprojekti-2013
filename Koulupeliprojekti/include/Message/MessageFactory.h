#pragma once
class Message;
#include <memory>
#include <SDL.h>
class Entity;

namespace MessageFactory
{
  std::unique_ptr<Message> CreateLocationChangeMessage(double x, double y);
  std::unique_ptr<Message> CreateCollisionMessage(Entity *entity, SDL_Rect &intersection);
}