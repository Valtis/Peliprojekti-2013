#pragma once
#include "Message/Message.h"

class QueryLocationMessage : public Message
{
public:
  QueryLocationMessage() : m_x(0), m_y(0) { }
  ~QueryLocationMessage() { }
  
  int GetX() const { return m_x; }
  int GetY() const { return m_y; }

  MessageType GetType() const override { return MessageType::QUERY_LOCATION; }

  void SetXY(int x, int y)
  {
    m_x = x;
    m_y = y;
  }

private:
  int m_x;
  int m_y;
};