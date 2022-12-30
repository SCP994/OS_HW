# CMakeList.txt : CMake project for HW03_01, include source and define
# project specific logic here.
#

# Add source to this project's executable.

find_package (Threads REQUIRED)

add_executable (msg "msg.c" "sender.c" "sender.h" "receiver.c" "receiver.h" "common.c" "common.h")

target_link_libraries(msg
    ${CMAKE_THREAD_LIBS_INIT}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET msg PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
