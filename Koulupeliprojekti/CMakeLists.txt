cmake_minimum_required (VERSION 2.6)
project (PELI)

set (
  SOURCES
  "src/Game.cpp"
  "src/Collision/CollisionChecker.cpp"
  "src/Component/AiComponent.cpp"
  "src/Component/CollisionComponent.cpp"
  "src/Component/Component.cpp"
  "src/Component/FactionComponent.cpp"
  "src/Component/FlyingAiComponent.cpp"
  "src/Component/FollowingAiComponent.cpp"
  "src/Component/GraphicsComponent.cpp"
  "src/Component/HealthComponent.cpp"
  "src/Component/InputComponent.cpp"
  "src/Component/LocationComponent.cpp"
  "src/Component/PhysicsComponent.cpp"
  "src/Component/VelocityComponent.cpp"
  "src/Component/WalkingAiComponent.cpp"
  "src/Component/Scripts/BulletScriptComponent.cpp"
  "src/Entity/Entity.cpp"
  "src/Entity/EntityFactory.cpp"
  "src/Graphics/FontManager.cpp"
  "src/Graphics/Renderer.cpp"
  "src/Graphics/Sprite.cpp"
  "src/Graphics/SpriteLoader.cpp"
  "src/Graphics/SpriteManager.cpp"
  "src/Graphics/SpriteSheetLoader.cpp"
  "src/Graphics/SurfaceOperations.cpp"
  "src/Graphics/TextureFactory.cpp"
  "src/Graphics/Camera/EntityTrackingCamera.cpp"
  "src/Level/Level.cpp"
  "src/Level/LevelManager.cpp"
  "src/Message/Message.cpp"
  "src/Message/MessageFactory.cpp"
  "src/Message/MessageProcessor.cpp"
  "src/Message/Commands/ControlCommand.cpp"
  "src/Physics/PhysicsHandler.cpp"
  "src/UI/InputManager.cpp"
  "src/UI/Controller/JoystickController.cpp"
  "src/UI/Controller/KeyboardController.cpp"
  "src/UI/Controller/MouseController.cpp"
  "src/UI/Window/Button.cpp"
  "src/UI/Window/TextBox.cpp"
  "src/UI/Window/TextElement.cpp"
  "src/UI/Window/TextField.cpp"
  "src/UI/Window/Window.cpp"
  "src/UI/Window/WindowBase.cpp"
  "src/UI/Window/WindowManager.cpp"
  "src/Utility/Logger.cpp"
  "src/Utility/LoggerManager.cpp"
  "src/Utility/StringUtility.cpp"
  "src/Utility/Tick.cpp"
  "main.cpp"
)

set (
  SDL_LIBRARY
  "C:\\Libs\\SDL\\SDL2-2.0.0\\lib\\x64\\SDL2.dll"
)
set (
  SDL_INCLUDE_DIR
  "C:\\Libs\\SDL\\SDL2-2.0.0\\include"
)

set (
  SDL_TTF_LIBRARIES
  "C:\\Libs\\SDL\\SDL2_ttf\\SDL2_ttf-2.0.12\\lib"
)
set (
  SDL_TTF_LIBRARY
  "C:\\Libs\\SDL\\SDL2_ttf\\SDL2_ttf-2.0.12\\lib\\x64\\SDL2_ttf.dll"
)
set (
  SDL_TTF_INCLUDE_DIR
  "C:\\Libs\\SDL\\SDL2_ttf\\SDL2_ttf-2.0.12\\include"
)
set (
  SDL_TTF_INCLUDE_DIRS
  "C:\\Libs\\SDL\\SDL2_ttf\\SDL2_ttf-2.0.12\\include"
)

Find_Package ( SDL REQUIRED )
Find_Package ( SDL_TTF REQUIRED )

if ( NOT SDL_FOUND )
  message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

list(
  APPEND
  CMAKE_CXX_FLAGS
  "-std=c++0x ${CMAKE_CXX_FLAGS} -g"
)

link_libraries (
  ${SDL_LIBRARY}
  ${SDL_TTF_LIBRARY}
  SDLmain # Sadly not included in SDL_LIBRARY variable
)

include_directories (
  ${PELI_SOURCE_DIR}/include
  ${SDL_INCLUDE_DIR}
  ${SDL_TTF_INCLUDE_DIR}
)

add_executable (
  Peli
  WIN32
  ${SOURCES}
)
