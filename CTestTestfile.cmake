# CMake generated Testfile for 
# Source directory: /Users/jevon/programs/emulators/nes
# Build directory: /Users/jevon/programs/emulators/nes
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(testAll "test_instructions")
set_tests_properties(testAll PROPERTIES  _BACKTRACE_TRIPLES "/Users/jevon/programs/emulators/nes/CMakeLists.txt;43;add_test;/Users/jevon/programs/emulators/nes/CMakeLists.txt;0;")
add_test(testAll, "test_utils")
set_tests_properties(testAll, PROPERTIES  _BACKTRACE_TRIPLES "/Users/jevon/programs/emulators/nes/CMakeLists.txt;50;add_test;/Users/jevon/programs/emulators/nes/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
