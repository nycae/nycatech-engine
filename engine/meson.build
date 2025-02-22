subdir('library')
subdir('renderer')
engine = static_library('nycatech', 'engine.cc',
                        install: true,
                        link_with: renderer,
                        dependencies: [sdl2, sdl2main, gl, glew, zstd_lib])