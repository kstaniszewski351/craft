
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

set(VCPKG_TARGET_TRIPLET "x64-linux")
include("$ENV{HOME}/.local/share/vcpkg/scripts/buildsystems/vcpkg.cmake")
include(FetchContent)

project(opengl)

find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(EnTT CONFIG REQUIRED)

# FetchContent_Declare(
#   glad
#   GIT_REPOSITORY https://github.com/Dav1dde/glad.git
# )
# FetchContent_MakeAvailable(glad)

add_executable(main
  src/main.cpp
  src/shader.cpp
  src/asset_manager.cpp
  src/text_file.cpp
  src/camera_controller.cpp
  src/chunk_mesh.cpp
  src/chunk.cpp
  src/fpv_camera.cpp
  src/shader.cpp
  src/world_renderer.cpp
  src/world.cpp
  src/atlas.cpp
  src/bitmap.cpp
  src/texture.cpp
  src/stb_impl.cpp
  src/buffer.cpp
  src/scene.cpp
  src/systems/hitbox_renderer.cpp
  src/systems/physics.cpp
  src/window.cpp
  src/input_manager.cpp
  src/systems/player_movement.cpp
  src/block_registry.cpp
  src/atlas_builder.cpp
  src/block.cpp
  src/chunk_gen.cpp
  src/frustrum.cpp
  src/init_gl.cpp
  src/game.cpp
  src/entity.cpp
  lib/glad/src/glad.c
)

target_include_directories(main PRIVATE
  ${Stb_INCLUDE_DIR}
  "lib/glad/include"
)

target_link_libraries(main PRIVATE
  SDL3::SDL3
  glm::glm
  EnTT::EnTT
)

