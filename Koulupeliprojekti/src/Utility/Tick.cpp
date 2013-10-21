#include "Utility/Tick.h"

Tick::Tick( Uint32 tickLength ) : m_lastTick(0), m_lastTickLength(0), m_tickLength(tickLength)
{

}

double Tick::TicksPassed()
{
  return m_lastTickLength/m_tickLength;
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


