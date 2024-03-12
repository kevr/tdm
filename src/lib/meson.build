if not get_option('disable_tests')
  lib_sys_test = executable('lib_sys_test',
    'sys.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('lib_sys test', lib_sys_test)
endif
