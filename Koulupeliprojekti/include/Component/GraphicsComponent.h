#pragma once
#include "Component/Component.h"
#include "Graphics/Sprite.h"
#include <unordered_map>
#include <vector>

class GraphicsComponent : public Component
{
public:

  GraphicsComponent();
  ~GraphicsComponent();

  void SetAnimation(int id);
  void NextFrame();
  void PreviousFrame();
  void AddFrame(int animationID, int frameID);
  void SetFrames(int animationID, std::vector<int> animationFrames);
  int GetSpriteID();

private:
  
  struct FrameData
  {
    FrameData() : mTicksToNextFrame(0), mCurrentTickCount(0), spriteID(0) {}
    double mTicksToNextFrame;
    double mCurrentTickCount;
    int spriteID;
  };

  int m_animationID;
  int m_frameID;
  std::unordered_map<int, std::vector<int>> m_animations;
};