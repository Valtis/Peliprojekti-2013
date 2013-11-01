#pragma once
#include "Message.h"

class Entity;

class TerminateEntityMessage : public Message
{
public:
  TerminateEntityMessage(Entity *terminateEntity) { m_terminateEntity = terminateEntity; }
  MessageType GetType() const override { return MessageType::TERMINATE_ENTITY; }
  Entity *GetTerminateEntity() { return m_terminateEntity; }
private:
  Entity *m_terminateEntity;
};
