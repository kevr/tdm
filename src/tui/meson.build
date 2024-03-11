if not get_option('disable_tests')
  tui_login_test = executable('tui_login_test',
    'login.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('tui_login test', tui_login_test)

  screen_test = executable('screen_test',
    'screen.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('screen test', screen_test)
endif
