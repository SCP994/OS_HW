# CMakeList.txt : CMake project for HW03_01, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (cmd1 "cmd1.c")
add_executable (cmd2 "cmd2.c")
add_executable (cmd3 "cmd3.c")
add_executable (shell "shell.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cmd1 PROPERTY CXX_STANDARD 20)
  set_property(TARGET cmd2 PROPERTY CXX_STANDARD 20)
  set_property(TARGET cmd3 PROPERTY CXX_STANDARD 20)
  set_property(TARGET shell PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
