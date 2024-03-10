project(
  'format-string-attack',
  'c',
  'cpp',
  version: '0.1',
  default_options: ['warning_level=3', 'cpp_std=gnu++23'],
)

flag = get_option('flag')

if flag == 'invalid'
	error('invalid flag')
endif

executable(
  'server',
  'main.cpp',
  'mongoose.c',
  install: true,
  cpp_args: [f'-DFLAG="@flag@"', '-fpermissive']
)
