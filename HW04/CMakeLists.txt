# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (fs "run.c" "run.h" "fs_cmd.c" "fs_cmd.h" "fs.c" "fs.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET fs PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
