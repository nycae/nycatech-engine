find_package(SDL3 REQUIRED)
find_package(glad REQUIRED)
find_package(zstd CONFIG REQUIRED)

add_library(
  nycatech
    config.cc
    ecs.cc
    engine.cc
    material.cc
    mesh.cc
    package.cc
    renderer.cc
    scene.cc
    tiny_obj_loader.cc
    transform.cc
)

target_include_directories(
  nycatech
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(
  nycatech
    PUBLIC
      SDL3::SDL3
      glad::glad
      zstd::libzstd
    PRIVATE
      protos
)