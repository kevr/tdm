sources = [
  'app.cpp',
]

gtest_proj = subproject('gtest')
gtest = gtest_proj.get_variable('gtest_dep')
gtest_main = gtest_proj.get_variable('gtest_main_dep')

curses = dependency('curses')

# static lib shared between tests
test_lib = static_library('test_lib',
  sources : sources)
test_dep = declare_dependency(
  link_with : [test_lib])
test_deps = [test_dep, curses, gtest, gtest_main]

# tests
main_test = executable('main_test',
  'main.test.cpp',
  dependencies : test_deps)
test('main test', main_test)

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
