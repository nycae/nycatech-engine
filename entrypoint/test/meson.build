tests = executable('tests', 'main.cc',
                    install: true,
                    link_with: [engine],
                    include_directories: engine_include)

test('test', tests)