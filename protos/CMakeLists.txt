find_package(flatbuffers CONFIG REQUIRED)
find_program(FLATC_EXECUTABLE flatc REQUIRED)

if(NOT FLATC_EXECUTABLE)
  message(FATAL_ERROR "FlatBuffers compiler (flatc) not found! Ensure FlatBuffers is installed via vcpkg.")
endif()

file(GLOB FLATBUFFERS_SCHEMAS "${CMAKE_CURRENT_SOURCE_DIR}/*.fbs")
set(GENERATED_DIR "${CMAKE_BINARY_DIR}/generated")

file(MAKE_DIRECTORY ${GENERATED_DIR})

set(GENERATED_HEADERS)
foreach(SCHEMA ${FLATBUFFERS_SCHEMAS})
  get_filename_component(SCHEMA_NAME ${SCHEMA} NAME_WE)
  set(GENERATED_HEADER "${GENERATED_DIR}/${SCHEMA_NAME}_generated.h")
  list(APPEND GENERATED_HEADERS ${GENERATED_HEADER})
  add_custom_command(
    OUTPUT ${GENERATED_HEADER}
    COMMAND ${FLATC_EXECUTABLE} --cpp -o ${GENERATED_DIR} ${SCHEMA}
    MAIN_DEPENDENCY ${SCHEMA}
    COMMENT "Generating FlatBuffers header for ${SCHEMA_NAME}.fbs"
  )
endforeach()

add_custom_target(genfb DEPENDS ${GENERATED_HEADERS})

add_library(protos INTERFACE)
target_include_directories(protos INTERFACE ${GENERATED_DIR})
target_link_libraries(protos INTERFACE flatbuffers::flatbuffers)

set(GENERATED_DIR ${GENERATED_DIR} PARENT_SCOPE)
set(FLATBUFFERS_HEADERS_LIB protos PARENT_SCOPE)
