#include "Game.h"

int main()
{
  try 
  {
    Game game;
    game.Run();
  }
  catch (const std::exception &e)
  {
	  // catch any uncaught exceptions - preferably no exception should reach this point
	  // log exception here 
	  return 1;
  }
  return 0;
}
