zstd_lib = dependency('libzstd')
flatbuffers_lib = dependency('flatbuffers')

nycatech_lib = static_library(
  'nycatech_lib',
  sources : [
    'config.cc',
    'transform.cc',
    'package.cc',
    'ecs.cc',
    'scene.cc',
    'mesh.cc',
    generated_files],
  install : true,
  dependencies : [zstd_lib, flatbuffers_lib])
nycatech_lib_include = include_directories('../')