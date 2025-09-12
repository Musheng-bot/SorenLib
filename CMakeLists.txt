cmake_minimum_required(VERSION 4.0)
project(SorenLib VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_library(SorenLib SHARED
		src/Thread.cpp
		src/Topic.cpp
		src/Logger.cpp
		src/LogDestination.cpp
		src/LogStream.cpp
)
target_include_directories(SorenLib PUBLIC include PRIVATE include/internal)

add_executable(test test.cpp
		src/LogDestination.cpp
		src/Logger.cpp
		include/internal/LogDestination.hpp
		src/LogStream.cpp
		include/internal/LogStream.hpp)
target_include_directories(test PRIVATE include include/internal)
target_link_libraries(test PRIVATE SorenLib)

file(GLOB HEADERS /include/*.hpp)
include(GNUInstallDirs)
install(FILES {HEADERS}
	DESTINATION {CMAKE_INSTALL_INCLUDEDIR}/SorenLib
)
install(TARGETS SorenLib
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

