cmake_minimum_required (VERSION 3.10)
project(clamp_cast VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GoogleTest)

# Require up-to-date compiler.
set(CMAKE_CXX_STANDARD 17)

# Output binary to predictable location (fixes cyclic dependency issues).
set(BINARY_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BINARY_OUT_DIR})

foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
	string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${BINARY_OUT_DIR})
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)


file(GLOB HEADER_FILES "${PROJECT_SOURCE_DIR}/include/clamp_cast/clamp_cast.hpp")
add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${HEADER_FILES}>
)
target_include_directories(${PROJECT_NAME} INTERFACE
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_compile_options(${PROJECT_NAME} INTERFACE -Wall -Wextra -Wpedantic -Werror)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
	# using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	target_compile_options(${PROJECT_NAME} INTERFACE /W4 /WX /EHsc /Zc:__cplusplus /FAs /permissive-)
	add_definitions(-DNOMINMAX)
endif()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.clang-format ${CMAKE_CURRENT_BINARY_DIR}/.clang-format COPYONLY)

# Install Package Configuration
set(CLAMP_CAST_INSTALL_CONFIGDIR "${CMAKE_INSTALL_LIBDIR}/cmake/clamp_cast")
install(FILES ${HEADER_FILES} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/clamp_cast")

write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/clamp_castConfigVersion.cmake"
	VERSION ${${PROJECT_NAME}_VERSION}
	COMPATIBILITY AnyNewerVersion
	)

configure_package_config_file(
	cmake/clamp_castConfig.cmake.in
	${CLAMP_CAST_INSTALL_CONFIGDIR}/clamp_castConfig.cmake
	INSTALL_DESTINATION ${CLAMP_CAST_INSTALL_CONFIGDIR}
	PATH_VARS CMAKE_INSTALL_INCLUDEDIR
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)

install(
	FILES
	"${CMAKE_CURRENT_BINARY_DIR}/${CLAMP_CAST_INSTALL_CONFIGDIR}/clamp_castConfig.cmake"
	"${CMAKE_CURRENT_BINARY_DIR}/clamp_castConfigVersion.cmake"
	DESTINATION ${CLAMP_CAST_INSTALL_CONFIGDIR}
	)

install(TARGETS ${PROJECT_NAME} EXPORT clamp_castTargets)

export(EXPORT clamp_castTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/clamp_castTargets.cmake"
	)

install(
	EXPORT clamp_castTargets FILE clamp_castTargets.cmake
	DESTINATION ${CLAMP_CAST_INSTALL_CONFIGDIR}
	)

export(PACKAGE ${PROJECT_NAME})

# Tests
option(BUILD_TESTING "Build and run tests." Off)
if (${BUILD_TESTING})
	enable_testing()
	
	include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
	conan_basic_setup(TARGETS)
	
	set(TEST_NAME ${PROJECT_NAME}_tests)
	file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp" "tests/*.c" "tests/*.hpp" "tests/*.h" "tests/*.tpp")
	add_executable(${TEST_NAME} ${TEST_SOURCES})
	target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME} CONAN_PKG::gtest CONAN_PKG::boost_multiprecision)
	gtest_discover_tests(${TEST_NAME})
	add_dependencies(${TEST_NAME} ${PROJECT_NAME})

	if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
		target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		target_compile_options(${TEST_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
		# using Intel C++
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		target_compile_options(${TEST_NAME} PRIVATE /W4 /WX /EHsc /Zc:__cplusplus /FAs /permissive-)
		add_definitions(-DNOMINMAX)
		set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TEST_NAME})
	endif()
endif()
