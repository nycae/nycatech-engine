client = executable('client', 'main.cc',
                    install: true,
                    win_subsystem: 'windows',
                    link_with: [engine],
                    include_directories: engine_include,
                    dependencies: [sdl2, sdl2main, gl, glew, zstd_lib])

test('test', client)