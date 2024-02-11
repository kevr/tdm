if not get_option('disable_tests')
  process_test = executable('process_test',
    'process.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('process test', process_test)

  passwd_test = executable('passwd_test',
    'passwd.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('passwd test', passwd_test)
endif
