sources = [
  'app.cpp',
]

ncurses = dependency('curses')

curses = dependency('curses')

# tdm executable
tdm_deps = [curses]
tdm_lib = static_library('tdm_lib',
  sources : sources,
  dependencies : tdm_deps)
exe_sources = ['main.cpp']
exe = executable('tdm',
  sources : exe_sources,
  link_with : [tdm_lib],
  dependencies : tdm_deps,
  install : true)
