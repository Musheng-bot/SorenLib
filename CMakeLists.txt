cmake_minimum_required(VERSION 3.22.1)
project(SorenLib VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(SorenLib SHARED
		src/Thread.cpp

		src/Logger.cpp
		src/LogDestination.cpp

		include/TopicInterface.hpp
		include/internal/Publisher.hpp
		include/internal/Subscriber.hpp
		include/internal/Topic.hpp

		include/Any.hpp
		include/Result.hpp
		include/Nullable.hpp
)
target_include_directories(SorenLib
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		$<INSTALL_INTERFACE:include>
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

add_executable(test
		test.cpp
)
target_include_directories(test PRIVATE include include/internal)
target_link_libraries(test PRIVATE SorenLib)

include(GNUInstallDirs)

install(TARGETS SorenLib
	EXPORT SorenLibTargets
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	INCLUDES DESTINATION include/SorenLib include/
)

install(DIRECTORY include/ DESTINATION include/SorenLib)
include(CMakePackageConfigHelpers)

# 生成配置文件模板（无需手动创建，通过函数自动处理）
configure_package_config_file(
		# 输入模板（使用 CMake 内置逻辑，无需实际文件）
		"${CMAKE_CURRENT_LIST_DIR}/config.cmake.in"
		# 输出文件（生成到构建目录）
		"${CMAKE_CURRENT_BINARY_DIR}/SorenLibConfig.cmake"
		# 安装路径中的路径变量替换
		INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SorenLib
		# 需要替换的路径变量（与 target_include_directories 中的对应）
		PATH_VARS CMAKE_INSTALL_INCLUDEDIR
)

install(
	EXPORT SorenLibTargets
	FILE SorenLibTargets.cmake  # 生成的文件名
	NAMESPACE SorenLib::        # 目标命名空间（推荐，避免冲突）
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SorenLib  # 安装路径（与配置文件同目录）
)

install(FILES
	"${CMAKE_CURRENT_BINARY_DIR}/SorenLibConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/SorenLibConfigVersion.cmake"
	DESTINATION lib/cmake/SorenLib
)

write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/SorenLibConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

