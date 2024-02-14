configure_file(input : 'config.h.in',
               output : 'config.h',
               configuration : conf_data)

sources = [
  'app.cpp',
  'freedesktop/desktop.cpp',
  'lib/curses.cpp',
  'lib/sys.cpp',
  'sys/passwd.cpp',
  'sys/process.cpp',
  'util/argparse.cpp',
  'util/datetime.cpp',
  'util/env.cpp',
  'util/filesystem.cpp',
  'util/logger.cpp',
  'util/str.cpp',
  'util/termio.cpp',
]
fmt_proj = subproject('fmt', default_options : 'default_library=static')
fmt = fmt_proj.get_variable('fmt_dep')

if not get_option('disable_tests')
  gtest_proj = subproject('gtest')
  gtest = gtest_proj.get_variable('gtest_dep')
  gtest_main = gtest_proj.get_variable('gtest_main_dep')
  gmock = gtest_proj.get_variable('gmock_dep')

  # static lib shared between tests
  test_sources = [
    'lib/stubs/curses.cpp'
  ]
  test_flags = ['-DTDM_TEST']
  test_lib_deps = [fmt, gtest, gmock]
  test_lib = static_library('test_lib',
    sources : sources + test_sources,
    cpp_args : test_flags,
    dependencies : test_lib_deps)
  test_dep = declare_dependency(
    link_with : [test_lib])

# dependencies to include when compiling .test.cpp units
  test_deps = test_lib_deps + [test_dep, gtest_main]
endif

# Recurse into util
subdir('util')
subdir('sys')
subdir('freedesktop')

# tests
if not get_option('disable_tests')
  main_test = executable('main_test',
    'main.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('main test', main_test)

  app_test = executable('app_test',
    'app.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('app test', app_test)
endif

# tdm executable
if not get_option('disable_bin')
  curses = dependency('curses')
  tdm_lib = static_library('tdm_lib',
    sources : sources,
    dependencies : [curses, fmt])
  tdm_dep = declare_dependency(
    link_with : [tdm_lib])

  exe = executable('tdm',
    sources : ['main.cpp'],
    dependencies : [tdm_dep, fmt],
    install : true)
endif
