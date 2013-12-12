#pragma once
#include "Message/Message.h"
#include "Utility/FactionEnum.h"

class QueryFactionMessage : public Message
{
public:
  QueryFactionMessage() { }
  ~QueryFactionMessage() { }

  Faction GetFaction() const { return m_faction; }
  void SetFaction(Faction f) { m_faction = f; }

  MessageType GetType() const override { return MessageType::QUERY_FACTION; }

private:
  Faction m_faction;
};