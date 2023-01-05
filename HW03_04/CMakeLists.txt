# CMakeList.txt : CMake project for HW03_01, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_executable (snd "sender.c" "common.c" "common.h")
add_executable (rcv "receiver.c" "common.c" "common.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET snd PROPERTY CXX_STANDARD 20)
  set_property(TARGET rcv PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
