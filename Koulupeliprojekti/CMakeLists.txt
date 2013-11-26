cmake_minimum_required (VERSION 2.6)
project (PELI)

set (ENV{PKG_CONFIG_PATH}
  "/home/jtpakari/Software/lib/pkgconfig"
)

set(EXECUTABLE_OUTPUT_PATH "bin")
set(LIBRARY_OUTPUT_PATH "bin")
set(CMAKE_CACHEFILE_DIR "cmake")

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
  "src/Level/LevelGenerator.cpp"
  "src/Message/Message.cpp"
  "src/Message/MessageFactory.cpp"
  "src/Message/MessageProcessor.cpp"
  "src/Message/Commands/ControlCommand.cpp"
  "src/Physics/PhysicsHandler.cpp"
  "src/Sound/Music.cpp"
  "src/Sound/SoundEffects.cpp"
  "src/Sound/SoundManager.cpp"
  "src/UI/Hud.cpp"
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

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)

set(
  CMAKE_CXX_FLAGS
  "${CMAKE_CXX_FLAGS}-std=c++0x -g -O3"
)

message (
  STATUS
  "${CMAKE_CXX_FLAGS}"
)

include_directories (
  SYSTEM
  ${SDL2_INCLUDE_DIRS}
  ${SDL2_TTF_INCLUDE_DIRS}
  ${SDL2_MIXER_INCLUDE_DIRS}
  ${PELI_SOURCE_DIR}/include
)

link_directories (
  ${SDL2_LIBRARY_DIRS}
  ${SDL2_TTF_LIBRARY_DIRS}
  ${SDL2_MIXER_LIBRARY_DIRS}
)

add_executable (
  Peli
  WIN32
  ${SOURCES}
)

target_link_libraries (
  Peli
  ${SDL2_LIBRARIES}
  ${SDL2_TTF_LIBRARIES}
  ${SDL2_MIXER_LIBRARIES}
)
