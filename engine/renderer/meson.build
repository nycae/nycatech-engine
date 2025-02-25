gl = dependency('gl')
glew = dependency('glew')
sdl2 = dependency('sdl2', static : true)
sdl2main = dependency('sdl2main')
renderer = static_library('renderer',
                          ['renderer.cc', 'tiny_obj_loader.cc'],
                          install : true,
                          include_directories : [nycatech_lib_include],
                          link_with : [nycatech_lib],
                          dependencies : [sdl2, sdl2main, gl, glew])