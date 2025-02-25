flatbuffers_proj = subproject('flatbuffers')
flatc = flatbuffers_proj.get_variable('flatc')
generated_files = custom_target(
  'generate_flatbuffers',
  input : 'scene.fbs',
  output : ['scene_generated.h'],
  command : [
    flatc,
    '--cpp',
    '--filename-ext',
    'cc',
    '-o',
    '@OUTDIR@',
    '@INPUT@'])