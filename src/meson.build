sources = [
  'main.cpp',
  'app.cpp',
]

ncurses = dependency('curses')

exe = executable('tdm',
  sources : sources,
  dependencies : [ncurses],
  install : true)

test('basic', exe)
