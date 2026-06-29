
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory -fsanitize-recover=memory -fno-omit-frame-pointer")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")

set(VCPKG_TARGET_TRIPLET "x64-linux")
include("$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
include(FetchContent)

project(opengl)

find_package(SDL3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_package(unofficial-nuklear CONFIG REQUIRED)
find_package(Freetype REQUIRED)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.8
  SOURCE_SUBDIR	cmake
)
# FetchContent_Declare(
#   nuklear
#   GIT_REPOSITORY https://github.com/Immediate-Mode-UI/Nuklear.git
#   GIT_TAG v4.13.3
# )

FetchContent_MakeAvailable(glad)

glad_add_library(glad STATIC API gl:core=4.6)

add_executable(main
  src/main.cpp
  src/gfx/shader.cpp
  src/text_file.cpp
  src/camera_controller.cpp
  src/chunk_mesh.cpp
  src/chunk.cpp
  src/fpv_camera.cpp
  src/world_renderer.cpp
  src/world.cpp
  src/atlas.cpp
  src/bitmap.cpp
  src/gfx/texture.cpp
  src/gfx/buffer.cpp
  src/scene.cpp
  src/systems/hitbox_renderer.cpp
  src/systems/physics.cpp
  src/window.cpp
  src/input_manager.cpp
  src/systems/player_movement.cpp
  src/blocks.cpp
  src/atlas_builder.cpp
  src/block.cpp
  src/chunk_gen.cpp
  src/frustrum.cpp
  src/init_gl.cpp
  src/game.cpp
  src/entity.cpp
  src/stb_impl.cpp
  src/nuklear_impl.cpp
  src/gfx/vao.cpp
  src/block_icon_gen.cpp
  src/gfx/framebuffer.cpp
  src/ortho_camera.cpp
  src/registries.cpp
  src/item.cpp
  src/components/view.cpp
  src/registry.cpp
  src/gui/gui_renderer.cpp
  src/gui/widget.cpp
  src/font_manager.cpp
  src/font.cpp
  src/gui/text.cpp
  src/gui/image.cpp
  src/gui/screen.cpp
  src/gui/hotbar.cpp
  src/texture_manager.cpp
  src/ecs.cpp
)

target_include_directories(main PRIVATE src)

target_link_libraries(main PRIVATE
  SDL3::SDL3
  glm::glm
  EnTT::EnTT
  glad
  PRIVATE Freetype::Freetype
)

