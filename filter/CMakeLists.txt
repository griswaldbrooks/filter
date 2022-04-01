cmake_minimum_required(VERSION 3.16)

project(filter)

add_compile_options(-std=c++17 -Wall -Wextra -Werror -Wno-unused-parameter -Wno-missing-field-initializers)

find_package(GTest REQUIRED)
include_directories(include ${GTEST_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}_tests
  test/linear_filter_tests.cpp
	)

target_include_directories(${PROJECT_NAME}_tests PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
	$<INSTALL_INTERFACE:include>
	)

target_link_libraries(${PROJECT_NAME}_tests
  ${GTEST_LIBRARIES}
  gmock
  pthread
	)
