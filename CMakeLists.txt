cmake_minimum_required(VERSION 4.0)
project(SorenLib VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_library(SorenLib SHARED
		include/Topic.hpp
		src/Thread.cpp
		include/Thread.hpp
		src/Topic.cpp
		src/Logger.cpp
		include/Logger.hpp
		test.cpp
)
target_include_directories(SorenLib PUBLIC include)

add_executable(test test.cpp)
target_include_directories(test PRIVATE include)
target_link_libraries(test PRIVATE SorenLib)

