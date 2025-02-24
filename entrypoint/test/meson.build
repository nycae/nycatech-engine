tests = executable('tests', 'main.cc',
                    install: true,
                    link_with: [nycatech_lib],
                    include_directories: include_directories('../../'))

test('test', tests)