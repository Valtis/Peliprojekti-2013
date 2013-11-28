#pragma once
#include <memory>
#include <vector>
#include <SDL.h>
//#include <iostream>

class Level;
class InputManager;
class EntityTrackingCamera;
class Hud;

class LevelGenerator {
  public:
    LevelGenerator();
    ~LevelGenerator();
    std::vector<std::unique_ptr<Level>> GenerateLevels(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera, Hud &hud);
  private:
    std::vector<SDL_Rect> GenerateGround(int steps);

    std::vector<SDL_Rect> GenerateBorders(int a, int b);
};
