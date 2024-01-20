sources = [
  'app.cpp',
  'lib/curses.cpp',
  'util/logger.cpp',
  'util/argparse.cpp',
  'util/termio.cpp',
]

gtest_proj = subproject('gtest')
gtest = gtest_proj.get_variable('gtest_dep')
gtest_main = gtest_proj.get_variable('gtest_main_dep')

# static lib shared between tests
test_sources = [
  'lib/stubs/curses.cpp'
]
test_lib = static_library('test_lib',
  sources : sources + test_sources,
  cpp_args : '-DTEST')
test_dep = declare_dependency(
  link_with : [test_lib])
test_deps = [test_dep, gtest, gtest_main]


# Recurse into util
subdir('util')

# tests
main_test = executable('main_test',
  'main.test.cpp',
  dependencies : test_deps)
test('main test', main_test)

# tdm executable
curses = dependency('curses')
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
