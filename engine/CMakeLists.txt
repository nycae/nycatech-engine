find_package(SDL3 REQUIRED)
find_package(glad REQUIRED)
find_package(glm REQUIRED)
find_package(zstd CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Stb REQUIRED)

add_library(
  nycatech
    engine.cc
    renderer.cc
    transform.cc
    shader.cc
    gltf_loader.cc
)

target_include_directories(
  nycatech
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/..
    PRIVATE
      ${Stb_INCLUDE_DIR}
)

target_link_libraries(
  nycatech
    PUBLIC
      SDL3::SDL3
      glad::glad
      zstd::libzstd
      ${Boost_LIBRARIES}
    PRIVATE
      protos
      glm::glm
)

target_compile_definitions(nycatech PRIVATE $<$<CONFIG:Debug>:DEBUG>)