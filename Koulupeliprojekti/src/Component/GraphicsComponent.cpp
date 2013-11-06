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
    std::stringstream ss;
    ss << id;
    LoggerManager::GetLog(COMPONENT_LOG)
      .AddLine(LogLevel::WARNING, 
      "Invalid animation id given to GraphicsComponent::SetAnimation (id "
      + ss.str() + ") - ignoring");
  }
}

void GraphicsComponent::AddFrame(int animationID, int spriteID)
{
  m_animations[animationID].push_back(spriteID);
  if (m_animationID == NO_VALUE)
  {
    m_animationID = animationID;
  }
  
  if (m_frameID == NO_VALUE)
  {
    m_frameID = 0;
  }
}

void GraphicsComponent::SetFrames(int animationID, std::vector<int> animationFrames)
{
  m_animations[animationID] = animationFrames;
  if (m_animationID == NO_VALUE)
  {
    m_animationID = animationID;
  }

  if (m_frameID == NO_VALUE)
  {
    m_frameID = 0;
  }
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
  if (m_frameID >= static_cast<int>(m_animations[m_animationID].size()))
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
