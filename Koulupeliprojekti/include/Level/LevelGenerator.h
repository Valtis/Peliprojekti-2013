#pragma once
#include <memory>
#include <vector>
#include <SDL.h>

class Level;
class InputManager;
class EntityTrackingCamera;

class LevelGenerator {
  public:
    LevelGenerator();
    ~LevelGenerator();
    std::vector<std::unique_ptr<Level>> generateLevels(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera);
  private:

    std::vector<SDL_Rect> generateGrid(int a, int b);
};
