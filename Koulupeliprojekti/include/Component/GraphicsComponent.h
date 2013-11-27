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
  void AddFrame(int animationID, int spriteID, double m_frameDelay);
  void AddFrame(int animationID, int spriteID) 
  {
    AddFrame(animationID, spriteID, 1);
  }
  //void SetFrames(int animationID, std::vector<int> animationFrames);
  int GetSpriteID();
  void Update(double m_ticksPassed) override;

  bool IsVisible() { return m_visible; }


private:
  void UpdateAnimation( double ticksPassed );

  struct AnimationData
  {
    int m_spriteID;
    double m_frameDelay;
  };

  double m_ticksPassed;
  
  int m_animationID;
  int m_frameID;
  bool m_visible;
  std::unordered_map<int, std::vector<AnimationData>> m_animations;
};