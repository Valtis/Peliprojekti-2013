#include "Game.h"
#include <exception>
#include "Utility/LoggerManager.h"
#include <string>

int main( int argc, char * argv[])
{
  try 
  {
    Game game;
    game.Run();
  }
  catch (const std::exception &ex)
  {
    LoggerManager::GetLog("error.log").AddLine(LogLevel::ERROR, std::string("Uncaught exception in main(): ") + ex.what());
    return 1;
  }
  return 0;
}
