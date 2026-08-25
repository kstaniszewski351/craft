
cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
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
  bgfx
  GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
  GIT_TAG v1.146.9317-553
)
FetchContent_MakeAvailable(
  bgfx
)

file(CREATE_LINK ${CMAKE_SOURCE_DIR}/res ${CMAKE_BINARY_DIR}/res SYMBOLIC)


bgfx_compile_shaders(
  TYPE VERTEX
  INCLUDE_DIRS ${BGFX_DIR}/src
  SHADERS shaders/chunk_vs.sc shaders/debug_vs.sc shaders/rect_vs.sc shaders/text_vs.sc shaders/atlas_vs.sc
  VARYING_DEF ${CMAKE_SOURCE_DIR}/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
)

bgfx_compile_shaders(
  TYPE FRAGMENT
  INCLUDE_DIRS ${BGFX_DIR}/src  
  SHADERS shaders/chunk_fs.sc shaders/debug_fs.sc shaders/rect_fs.sc shaders/text_fs.sc
  VARYING_DEF ${CMAKE_SOURCE_DIR}/shaders/varying.def.sc
  OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders
)

add_executable(main
  src/main.cpp
  src/file.cpp
  src/camera_controller.cpp
  src/chunk_mesh.cpp
  src/chunk.cpp
  src/fpv_camera.cpp
  src/world_renderer.cpp
  src/world.cpp
  src/atlas.cpp
  src/bitmap.cpp
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
  src/game.cpp
  src/entity.cpp
  src/stb_impl.cpp
  src/nuklear_impl.cpp
  src/block_icon_gen.cpp
  src/ortho_camera.cpp
  src/registries.cpp
  src/item.cpp
  src/components/view.cpp
  src/registry.cpp
  src/gui/gui_renderer.cpp
  src/gui/widget.cpp
  src/font_manager.cpp
  src/font.cpp
  src/gui/image.cpp
  src/gui/screen.cpp
  src/gui/hotbar.cpp
  src/gui/text.cpp
  src/texture_manager.cpp
  src/shader_manager.cpp
  src/init_bgfx.cpp
  src/ecs.cpp
  shaders/chunk_vs.sc
  shaders/debug_vs.sc
  shaders/rect_vs.sc
  shaders/text_vs.sc
  shaders/chunk_fs.sc
  shaders/debug_fs.sc
  shaders/rect_fs.sc
  shaders/text_fs.sc
  shaders/atlas_vs.sc
  src/load_texture.cpp
  src/gui/gui_manager.cpp
  src/gui/item_slot.cpp
)

target_include_directories(main PRIVATE src)

target_link_libraries(main PRIVATE
  SDL3::SDL3
  glm::glm
  EnTT::EnTT
  Freetype::Freetype
  bgfx
)

