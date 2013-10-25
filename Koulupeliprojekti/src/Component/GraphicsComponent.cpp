#include "Component/GraphicsComponent.h"
#include "Utility/LoggerManager.h"
const int NO_VALUE = -1;

GraphicsComponent::GraphicsComponent() : m_animationID(NO_VALUE), m_frameID(NO_VALUE)
{

}

GraphicsComponent::~GraphicsComponent()
{

}

void GraphicsComponent::SetAnimation(int id)
{
  if (m_animations.count(id) != 0)
  {
    m_animationID = id;
  }
  else
  {
    LoggerManager::GetLog(COMPONENT_LOG)
      .AddLine(LogLevel::WARNING, 
      "Invalid animation id given to GraphicsComponent::SetAnimation (id "
      + std::to_string(id) + ") - ignoring");
  }
}

void GraphicsComponent::AddFrame(int animationID, int frameID)
{
  m_animations[animationID].push_back(frameID);
}

void GraphicsComponent::SetFrames(int animationID, std::vector<int> animationFrames)
{
  m_animations[animationID] = animationFrames;
}

int GraphicsComponent::GetSpriteID()
{
  if (m_animationID == NO_VALUE || m_frameID == NO_VALUE)
  {
    return NO_VALUE;
  }
  return m_animations[m_animationID][m_frameID];
}

void GraphicsComponent::NextFrame()
{
  if (m_animationID == NO_VALUE)
  {
    return;
  }

  ++m_frameID;
  if (m_frameID >= m_animations[m_animationID].size())
  {
    m_frameID = 0;
  }
}

void GraphicsComponent::PreviousFrame()
{
  if (m_animationID == NO_VALUE)
  {
    return;
  }

  --m_frameID;
  if (m_frameID < 0)
  {
    m_frameID = m_animations[m_animationID].size() - 1;
  }
}