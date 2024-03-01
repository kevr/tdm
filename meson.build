project('tdm', 'cpp',
  version : '0.1',
  default_options : [
    'warning_level=3',
    'cpp_std=c++20'
  ])

conf_data = configuration_data()
conf_data.set('version', meson.project_version())

test_env = environment()
root = meson.source_root()
test_env.set('PAM_CONFDIR', root + '/pam.d')

subdir('src')
