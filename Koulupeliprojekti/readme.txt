How to run the game:
  Windows: 
    Run the game.exe. Microsoft visual studio 2012 c++ runtime is required to run the game. 
    If you do not have this runtime, you can download it from http://www.microsoft.com/en-us/download/details.aspx?id=30679
  Linux\OSX: 
    No binaries are provided. If you can be bothered, you can compile the game yourself.
    Source available at https://github.com/Valtis/Peliprojekti-2013.
    Requires SDL 2.0, SDL_ttf 2.0 and SDL_mixer 2.0 libraries.
    Download the appropriate development binaries
      SDL 2.0: http://www.libsdl.org/download-2.0.php
      SDL_mixer: http://www.libsdl.org/projects/SDL_mixer/ 
      SDL_ttf: http://www.libsdl.org/projects/SDL_ttf/

Keys:
  Left/Right- move and change aim direction right/left
  Up/Down - change aim direction up/down
  Space: Jump
  Left Shift: Fire

Goal: 
  Hit the door with either the player character or bullet

Known issues:
  Collision detection can be bit wonky. In particular, you may shoot through walls if you hug them, and push entities through walls by jumping on them.
  Rare crash when entity dies.  

Folders provided:
  logs: Contains log files that may be generated.
  src: source files
  include: header files
  data: data files used by the game
  docs: UML diagram and overall description of the game architecture 