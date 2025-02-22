client = executable('client', 'main.cc',
                    install: true,
                    win_subsystem: 'windows',
                    link_with: [engine, renderer],
                    include_directories: include_directories('../../'),
                    dependencies: [sdl2, sdl2main, gl, glew, zstd_lib])

test('test', client)