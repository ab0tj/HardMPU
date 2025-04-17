
# This CMakeLists.txt is generated.
# Do not modify directly.
# If you want to customize cmake behavior create a user.cmake in this
# directory and add it to your version control system.

if (EXISTS ${CMAKE_CURRENT_LIST_DIR}/user.cmake)
    include(user.cmake)
endif()

# format v1.00
# {

cmake_minimum_required(VERSION 3.24.0)
set(CMAKE_SYSTEM_NAME "Generic")

project(HardMPU_Release_project LANGUAGES C CXX ASM)

include("${CMAKE_CURRENT_LIST_DIR}/.generated/main.cmake")

# }

