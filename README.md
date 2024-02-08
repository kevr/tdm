# **t**extual **d**esktop **m**anager

![tests](https://github.com/kevr/tdm/actions/workflows/test.yaml/badge.svg?branch=main)

A simple desktop manager with a textual login dialog.

## Compilation

Setup a build directory with meson, optionally disabling tests:

    $ meson setup [-Ddisable_tests=true] build

Compile the application with `ninja`:

    $ ninja -C build

## Running tests

Built tests can be run using the `test` target:

    $ ninja -C build test

Refer to [TESTING.md](TESTING.md) for test documentation including coverage.

## Usage

After [compilation](#compilation), execute the resulting `tdm` binary:

    $ ./build/src/tdm

## Upstreams

All public upstreams' main branches are kept up to date when changes
are merged in from others.

- https://git.sr.ht/~kevr/tdm
- https://codeberg.org/kevr/tdm
- https://github.com/kevr/tdm
