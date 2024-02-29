if not get_option('disable_tests')
  pam_session_test = executable('pam_session_test',
    'session.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('pam_session test', pam_session_test)
endif
