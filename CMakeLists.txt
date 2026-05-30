
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
  src/shader.h
  src/asset_manager.cpp
  src/asset_manager.h
  src/asset.h
  src/asset_meta.h
  src/text_file.cpp
  src/text_file.h
  src/camera_controller.cpp
  src/camera_controller.h
  src/chunk_mesh.cpp
  src/chunk_mesh.h
  src/chunk.cpp
  src/chunk.h
  src/direction.h
  src/fpv_camera.h
  src/fpv_camera.cpp
  src/shader.cpp
  src/shader.h
  src/world_renderer.h
  src/world_renderer.cpp
  src/world.cpp
  src/world.h
  src/atlas.cpp
  src/atlas.h
  src/bitmap.cpp
  src/bitmap.h
  src/texture.cpp
  src/texture.h
  src/stb_impl.cpp
  src/buffer.cpp
  src/buffer.h
  src/scene.h
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

