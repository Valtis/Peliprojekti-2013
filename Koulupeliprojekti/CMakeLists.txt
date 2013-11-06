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
  "src/Component/Scripts/EndLevelScriptComponent.cpp"
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
  PC_SDL2_INCLUDE_DIRS
  "C:\\Libs\\SDL\\SDL2-2.0.0\\include"
)
set (
  PC_SDL2_TTF_INCLUDE_DIRS
  "C:\\Libs\\SDL\\SDL2_ttf\\SDL2_ttf-2.0.12\\include"
)

set (
  PC_SDL2_LIBRARY_DIRS
  "C:\\Libs\\SDL\\SDL2-2.0.0\\lib"
)
set (
  PC_SDL2_TTF_LIBRARY_DIRS
  "C:\\Libs\\SDL\\SDL2_ttf\\SDL2_ttf-2.0.12\\lib"
)

link_directories(
  ${PC_SDL2_LIBRARY_DIRS}
  ${PC_SDL2_TTF_LIBRARY_DIRS}
)

find_library (SDL2
  NAMES SDL2
  HINTS
    ${PC_SDL2_LIBRARY_DIRS}
  PATH_SUFFIXES x86
)

find_library (SDL2_TTF 
  NAMES SDL2_ttf
  HINTS
    ${PC_SDL2_TTF_LIBRARY_DIRS}
  PATH_SUFFIXES x86
)

find_library(SDL2_MAIN
  NAMES SDL2main
  HINTS
    ${PC_SDL2_LIBRARY_DIRS}
  PATH_SUFFIXES x86
)

set(SDL2_LIBRARIES
  ${SDL2_MAIN} #"SDL2main"
  ${SDL2} #"SDL2"
  ${SDL2_TTF} #"SDL2_ttf"
)

list(
  APPEND
  CMAKE_CXX_FLAGS
  "-std=c++0x ${CMAKE_CXX_FLAGS} -g"
)

find_path(SDL2_INCLUDE_DIR
  NAMES SDL.h
  HINTS
    ${PC_SDL2_INCLUDE_DIRS}
  PATH_SUFFIXES SDL2
)

find_path(SDL2_TTF_INCLUDE_DIR
  NAMES SDL_ttf.h
  HINTS
    ${PC_SDL2_TTF_INCLUDE_DIRS}
  PATH_SUFFIXES SDL2
)

set(SDL2_INCLUDE_DIRS
  ${SDL2_INCLUDE_DIR}
  ${SDL2_TTF_INCLUDE_DIR}
)

include_directories (
  ${SDL2_INCLUDE_DIRS}
  ${PELI_SOURCE_DIR}/include
)

add_executable (
  Peli
  WIN32
  ${SOURCES}
)

target_link_libraries (
  Peli
  ${SDL2_LIBRARIES}
)
