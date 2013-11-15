#include "Utility/Tick.h"

#ifndef min
#define min(a, b) (a) < (b) ? (a) : (b)
#endif

Tick::Tick( Uint32 tickLength ) : m_maxTickLength(2), m_lastTick(0), m_lastTickLength(0), m_tickLength(tickLength)
{

}

double Tick::TicksPassed()
{
  return min(m_lastTickLength/m_tickLength, m_maxTickLength);
}

bool Tick::TickHasPassed()
{
  auto currentTick = SDL_GetTicks();
  if (currentTick > m_lastTick + m_tickLength)
  {
    m_lastTickLength = currentTick - m_lastTick;
    m_lastTick = currentTick;
    return true;
  }
  return false;
}


