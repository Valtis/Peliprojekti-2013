#include "Component/GraphicsComponent.h"
#include "Entity/Entity.h"
#include "Utility/LoggerManager.h"
#include "Message/SetGraphicsVisibilityMessage.h"
#include "Message/AnimationChangeMessage.h"
const int NO_VALUE = -1;

GraphicsComponent::GraphicsComponent() : m_ticksPassed(0), m_animationID(NO_VALUE), m_frameID(NO_VALUE), m_visible(true)
{

}

GraphicsComponent::~GraphicsComponent()
{

}

void GraphicsComponent::Update(double ticksPassed)
{
  if (m_animationID == NO_VALUE || m_frameID == NO_VALUE)
  {
    return;
  }

  UpdateAnimation(ticksPassed);
}

void GraphicsComponent::UpdateAnimation( double ticksPassed )
{
  m_ticksPassed += ticksPassed;
  if (m_ticksPassed > m_animations[m_animationID][m_frameID].m_frameDelay)
  {
    
    m_ticksPassed = 0;
    NextFrame();
  }
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

void GraphicsComponent::AddFrame(int animationID, int spriteID, double frameDelay)
{
  AnimationData data;
  data.m_spriteID = spriteID;
  data.m_frameDelay = frameDelay;

  m_animations[animationID].push_back(data);
  
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
  return m_animations[m_animationID][m_frameID].m_spriteID;
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

void GraphicsComponent::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::CHANGE_ANIMATION_MESSAGE, Priority::LOWEST, 
    [=](Message *msg) { return this->HandleAnimationChangeMessage(msg); }    );

  GetOwner()->RegisterMessageHandler(MessageType::SET_GRAPHICS_VISIBILITY, Priority::NORMAL, 
    [=](Message *msg)
    {
      return this->HandleSetGraphicsVisibilityMessage(msg);
    });
}

MessageHandling GraphicsComponent::HandleSetGraphicsVisibilityMessage(Message *msg)
{
  auto visMsg = static_cast<SetGraphicsVisibilityMessage *>(msg);
  m_visible = visMsg->GetVisibility();
  return MessageHandling::STOP_HANDLING;
}

MessageHandling GraphicsComponent::HandleAnimationChangeMessage( Message * msg )
{
  auto animMsg = static_cast<AnimationChangeMessage *>(msg);
  if (m_animations.count(animMsg->GetId()) != 0 && animMsg->GetId() != m_animationID)
  {
    m_animationID = animMsg->GetId();
    m_frameID = 0;
  }
  return MessageHandling::STOP_HANDLING;
}

