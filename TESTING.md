Testing
-------

The project maintains a set of unit tests included in the meson build
by default. It can be finely tuned for test or production purposes,
see [build configuration](#configuring-meson) on top of
[meson built-in options](https://mesonbuild.com/Builtin-options.html).

### Build Configuration

Two custom options are defined in this project's `meson_options.txt`:

- `disable_bin (default: false)`
    - When `true`, the *tdm* binary is skipped
- `disable_tests (default: false)`
    - When `true`, unit tests are skipped

Along with a meson built-in option that can be set for coverage data:

- `b_coverage (default: false)`
    - When `true`, coverage data is generated on binary execution

### Developer Notes

When submitting patches to this project, you should ensure that 100% test
coverage is maintained. It is each developer's responsibility to test their
own features or fixes to the project.

Contributions not adhering to this requirement will be rejected out of
principle, as they would be putting unnecessary burdens on others.

### Example Workflow

    ## Configure a build for unit tests only with coverage data
    $ meson setup -Ddisable_bin=true -Db_coverage=true build

    ## Compile tests...
    $ ninja -C build

    ## Run tests...
    $ ninja -C build tests

    ## Generate coverage text report
    $ ninja -C build coverage-text
    $ cat build/meson-logs/coverage.txt

    ## Generate coverage XML cobertura report
    $ ninja -C build coverage-xml
    $ cat build/meson-logs/coverage.xml

    ## Generate coverage HTML report
    $ ninja -C build coverage-html
    $ $BROWSER build/meson-logs/coveragereport/index.html
