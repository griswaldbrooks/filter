list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

find_package(GTest 1.13.0 REQUIRED)
find_package(Threads REQUIRED)

add_executable(test_filter linear_filter_tests.cpp)
target_link_libraries(test_filter PRIVATE
  filter::filter
  GTest::gtest
  GTest::gmock
  Threads::Threads
)
gtest_discover_tests(test_filter)
