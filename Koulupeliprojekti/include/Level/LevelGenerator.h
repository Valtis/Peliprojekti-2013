#pragma once
#include <vector>
#include "Level/LevelManager.h"
#include "Level/Level.h"
#include "UI/InputManager.h"
#include <memory>
#include "Graphics/Camera/EntityTrackingCamera.h"
#include "Entity/Entity.h"
#include "Message/MessageFactory.h"
#include "Component/InputComponent.h"
#include "Component/GraphicsComponent.h"
#include "Component/LocationComponent.h"
#include "Component/VelocityComponent.h"
#include "Component/CollisionComponent.h"
#include "Component/FactionComponent.h"
#include "Component/FlyingAiComponent.h"
#include "Component/WalkingAiComponent.h"
#include "Component/FollowingAiComponent.h"
#include "Component/Scripts/EndLevelScriptComponent.h"
#include "Component/PhysicsComponent.h"
#include <stdlib.h>
#include <time.h>
#include "Level/LevelGenerator.h"
#include <vector>

struct MapRect {
    int x;
    int y;
    int h;
    int w;
};

class LevelGenerator {
  public:
    LevelGenerator();
    ~LevelGenerator();
    std::vector<std::unique_ptr<Level>> generateLevels(InputManager& m_inputManager, std::unique_ptr<EntityTrackingCamera>& camera);
  private:
    std::unique_ptr<Entity> CreatePlayer(int frame, int x, int y, int size, std::unique_ptr<InputComponent>& ci);
    void CreateEnemy(int frame, int x, int y, int size, std::unique_ptr<Level>& level, std::unique_ptr<AiComponent>& ai, std::unique_ptr<InputComponent>& ci);
    void CreateBlock(int frame, int x, int y, int size, std::unique_ptr<Level>& level, bool hitbox);
    void CreateEndLevelEntity(int frame, int x, int y, int size, std::unique_ptr<Level>& level);
    std::vector<MapRect> generateGrid(int a, int b);
    void CreateCollisionBlock(int x, int y, int w, int h, std::unique_ptr<Level>& level);

};
