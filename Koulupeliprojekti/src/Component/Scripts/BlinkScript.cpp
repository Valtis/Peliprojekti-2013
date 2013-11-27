#include "Component/Scripts/BlinkScript.h"
#include "Message/StartBlinkingMessage.h"
#include "Message/MessageProcessor.h"
#include "Entity/Entity.h"
void BlinkScript::OnAttatchingToEntity()
{
  GetOwner()->RegisterMessageHandler(MessageType::START_BLINKING, Priority::HIGH, 
    [=](Message *msg)
    {
      return this->HandleStartBlinkingMessage(msg);
    }
    );
}

void BlinkScript::Update(double ticksPassed)
{
  if (m_blinkDuration > 0)
  {
    m_blinkDuration -= ticksPassed;
    if (m_blinkDuration <= 0)
    {
      StopBlinking();
      return;
    }

    m_ticksUntilFlip -= ticksPassed;
    if (m_ticksUntilFlip <= 0)
    {
      FlipGraphicsState();

    }
  }
}

void BlinkScript::StopBlinking()
{
  auto msg = MessageFactory::CreateSetGraphicsVisibilityMessage(true);
  GetOwner()->SendMessage(msg.get());

  m_ticksUntilFlip = 0;
  m_blinkDuration = 0;
}


void BlinkScript::FlipGraphicsState()
{
  m_ticksUntilFlip = m_singleBlinkDuration;
  m_lastState = !m_lastState;
  auto msg = MessageFactory::CreateSetGraphicsVisibilityMessage(m_lastState);
  GetOwner()->SendMessage(msg.get());
}


MessageHandling BlinkScript::HandleStartBlinkingMessage(Message *message)
{
  auto blinkMsg = static_cast<StartBlinkingMessage *>(message);
  m_blinkDuration = blinkMsg->GetBlinkDuration();
  m_lastState = false;
  auto msg = MessageFactory::CreateSetGraphicsVisibilityMessage(m_lastState);
  GetOwner()->SendMessage(msg.get());
  return MessageHandling::STOP_HANDLING;
}
