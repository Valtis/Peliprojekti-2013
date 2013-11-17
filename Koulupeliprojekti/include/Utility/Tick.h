#pragma once
#include <SDL.h>

class Tick
{
public:
  Tick(Uint32 tickLength);

  double TicksPassed();
  bool TickHasPassed();
  double SetMaxTixLength(double length) { m_maxTickLength = length; }
private:
  Uint32 m_maxTickLength;
  Uint32 m_lastTick;
  Uint32 m_lastTickLength;

  Uint32 m_tickLength;
};