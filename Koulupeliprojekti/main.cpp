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
    LoggerManager::GetLog("error.log").AddLine(LogLevel::ERROR, std::string("Exception caught in main(): ") + ex.what());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Fatal error", ex.what(), nullptr);

    return 1;
  }
  return 0;
}
