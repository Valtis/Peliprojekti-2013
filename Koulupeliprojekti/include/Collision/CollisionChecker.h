#pragma once
#include <memory>
#include <vector>
#include <SDL.h>
#include <SDL_thread.h>

class Entity;

class CollisionChecker
{
  public:
    CollisionChecker() { InitThreadPool(SDL_GetCPUCount()); };
    ~CollisionChecker();

    void CheckCollisions(const std::vector<std::unique_ptr<Entity>> &entities,
                         const std::vector<std::unique_ptr<Entity>> &staticEntities);

  private:
    struct CollisionThread
    {
      SDL_Thread *thread;
      int id;
    };
    std::vector<std::unique_ptr<CollisionThread>> m_threads;

    InitThreadPool(int size);
}
