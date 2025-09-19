
# throwinfo

How to Simplify the debugging in case of exceptions:

## Presentation

The presentation: https://cppcon2025.sched.com/event/27bOE/enhancing-exception-handling-and-debugging-using-c++23-stdstacktrace

The slides from CppCon 2025 are in the docs/ directory.

How to capture stack trace from within the throw statement implementation?

Example code and library.

## Build and Test

To build there are two script you can choose from:
scripts/.

scripts/build-cmake.sh   -- build, using cmake and ninja, the binaries and run the demo
scripts/build-manual.sh  -- build, using make, the binaries and run the demo

# The Compiler explorer examples

The different compiler explorer examples

* slides-examples/35_throw_interception/intercept_throw.cpp:                                  https://compiler-explorer.com/z/TnsrcvWda
* slides-examples/35_throw_interception/exception_capture.cpp:                                https://compiler-explorer.com/z/Y7zrq9EPj
* slides-examples/60_window_throw_interception/Windows_capture_stacktrace_20250913a.cpp:      https://compiler-explorer.com/z/3q1rPG85G
* slides-examples/60_window_throw_interception/windows-compiler-flags.txt:                    https://compiler-explorer.com/z/3q1rPG85G
* slides-examples/60_window_throw_interception/example_windows_exception_handler.cpp:         https://compiler-explorer.com/z/Eqcjnb5YE
* slides-examples/60_window_throw_interception/example_windows_exception_handler.cpp:         https://compiler-explorer.com/z/srevj1rxe
* slides-examples/10_stacktrace/example_stacktrace_iterate.cpp:                               https://compiler-explorer.com/z/Kqb9sWK4E


