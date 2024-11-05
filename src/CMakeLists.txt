cmake_minimum_required(VERSION 3.5)

project(klsf)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build configuration: " ${CMAKE_BUILD_TYPE})

set(CMAKE_CXX_FLAGS "-Wall -g -ggdb -Wno-unknown-pragmas -std=c++20")
include_directories(SYSTEM ./fmt/include)

set(Boost_NO_SYSTEM_PATHS ON)
find_package(Boost 1.70.0 REQUIRED COMPONENTS program_options timer system chrono)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(cbfs cbfs.cpp graph.cpp random.cpp logging.cpp cbfs-options.cpp solution.cpp)
target_link_libraries(cbfs ${Boost_LIBRARIES} rt stdc++fs)

add_executable(exhaust exhaust.cpp graph.cpp random.cpp logging.cpp exhaust-options.cpp solution.cpp)
target_link_libraries(exhaust ${Boost_LIBRARIES} rt stdc++fs)
