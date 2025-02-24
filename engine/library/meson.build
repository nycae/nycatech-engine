zstd_lib = dependency('libzstd')
nycatech_lib = static_library('nycatech_lib',
                              ['config.cc', 'transform.cc', 'package.cc', 'ecs.cc', 'scene.cc'],
                              install : true,
                              dependencies: [zstd_lib])
nycatech_lib_include = include_directories('../')