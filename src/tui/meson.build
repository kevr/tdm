if not get_option('disable_tests')
  screen_test = executable('screen_test',
    'screen.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('screen test', screen_test)
endif
