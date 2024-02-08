# **t**extual **d**esktop **m**anager

![tests](https://github.com/kevr/tdm/actions/workflows/test.yaml/badge.svg?branch=main)

A simple desktop manager with a textual login dialog.

## Compilation

(1) Setup a build directory with meson:

    $ meson setup [-Db_coverage=true] build

(2) Compile the application using `samu` or `ninja`:

    $ ninja -C build

## Usage

After [compilation](#compilation), execute the resulting `tdm` binary:

    $ ./build/src/tdm

## Running tests

After [compilation](#compilation), tests can be run through the `test` target:

    $ ninja -C build test

## Test coverage

To configure [compilation](#compilation) for test coverage, include the
`-Db_coverage=true` flag:

    $ meson setup -Db_coverage=true build

After [running tests](#running-tests), coverage can be generated:

#### Text coverage report (depends: gcovr)

    $ ninja -C build coverage-text

#### XML coverage report (depends: lcov)

    $ ninja -C build coverage-xml

#### HTML coverage report (depends: lcov)

    $ ninja -C build coverage-html

## Upstreams

All public upstreams' main branches are kept up to date when changes
are merged in from others.

- https://git.sr.ht/~kevr/tdm
- https://codeberg.org/kevr/tdm
- https://github.com/kevr/tdm
