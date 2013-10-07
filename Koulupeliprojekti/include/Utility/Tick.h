#pragma once
#include <SDL.h>

class Tick
{
  Tick(Uint32 tickLength);

  double TicksPassed();
  bool TickHasPassed();

private:
  Uint32 m_lastTick;
  Uint32 m_lastTickLength;

  Uint32 m_tickLength;
};