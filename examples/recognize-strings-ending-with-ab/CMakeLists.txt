cmake_minimum_required(VERSION 3.10)
project(recognize_strings_ending_with_ab)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Use Clang as the compiler
set(CMAKE_C_COMPILER clang)

# Set strict compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")

# Include directories for FSM and string libraries
include_directories(../../core)
include_directories(../../lib/safety/sstring/core)

# Build the safe strings library
add_library(sstring STATIC ../../lib/safety/sstring/core/sstring.c)

# Build the FSM library
add_library(fsm_lib STATIC ../../core/fsm.c)
target_link_libraries(fsm_lib sstring)

# Build the example executable
add_executable(recognize_strings_ending_with_ab main.c)
target_link_libraries(recognize_strings_ending_with_ab fsm_lib sstring)