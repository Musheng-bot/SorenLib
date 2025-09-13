cmake_minimum_required(VERSION 4.0)
project(SorenLib VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

add_library(SorenLib SHARED
		src/Thread.cpp
		src/Logger.cpp
		src/LogDestination.cpp
		include/internal/Publisher.hpp
		include/internal/Subscriber.hpp
		include/TopicInterface.hpp
		include/internal/Topic.hpp
)
target_include_directories(SorenLib
	PUBLIC
		include
	PRIVATE
		include/internal)

add_executable(test test.cpp
		src/LogDestination.cpp
		src/Logger.cpp
		include/internal/LogDestination.hpp
)
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

