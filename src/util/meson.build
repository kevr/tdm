if not get_option('disable_tests')
  argparse_test = executable('argparse_test',
    'argparse.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('argparse test', argparse_test)

  env_test = executable('env_test',
    'env.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('env test', env_test)

  filesystem_test = executable('filesystem_test',
    'filesystem.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('filesystem test', filesystem_test)

  logger_test = executable('logger_test',
    'logger.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('logger test', logger_test)

  str_test = executable('str_test',
    'str.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('str test', str_test)

  termio_test = executable('termio_test',
    'termio.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('termio test', termio_test)
endif
