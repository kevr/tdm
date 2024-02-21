if not get_option('disable_tests')
  socket_stream_test = executable('socket_stream_test',
    'stream.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('socket_stream test', socket_stream_test)

  socket_nonblocking_stream_test = executable('socket_nonblocking_stream_test',
    'nonblocking_stream.test.cpp',
    cpp_args : test_flags,
    dependencies : test_deps)
  test('socket_nonblocking_stream test', socket_nonblocking_stream_test)
endif
