cmake_minimum_required(VERSION 3.13)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
project(bientropy-test)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(libbient)
include_directories(.)

add_executable(bientropy-test bient-test.cpp)

target_compile_options ( bientropy-test PUBLIC "-m64" )
target_link_options( bientropy-test PUBLIC "-m64")

#target_compile_options ( bientropy-test PUBLIC "-m32" )
#target_link_options( bientropy-test PUBLIC "-m32")
