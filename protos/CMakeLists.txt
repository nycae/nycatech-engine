find_package(Protobuf REQUIRED)
if (TARGET protobuf::libprotobuf)
  set_property(TARGET protobuf::libprotobuf PROPERTY CXX_STANDARD 17)
endif()

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/scene.proto")

add_library(protos ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protos PUBLIC protobuf::libprotobuf)
target_include_directories(protos PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
