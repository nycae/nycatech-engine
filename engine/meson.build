zstd_lib = dependency('libzstd')
flatbuffers_lib = dependency('flatbuffers')
gl = dependency('gl')
glew = dependency('glew')
sdl2 = dependency('sdl2', static : true)
sdl2main = dependency('sdl2main')

engine = static_library(
  'engine',
  sources : [
    'config.cc',
    'transform.cc',
    'package.cc',
    'ecs.cc',
    'scene.cc',
    'mesh.cc',
    'tiny_obj_loader.cc',
    'renderer.cc',
    'engine.cc',
    generated_files],
  install : true,
  dependencies : [zstd_lib, flatbuffers_lib, sdl2, sdl2main, gl, glew])
engine_include = include_directories('../')