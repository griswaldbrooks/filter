find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(test_filter linear_filter_tests.cpp)
target_link_libraries(test_filter PRIVATE
  filter::filter
  GTest::GTest
  gmock
  Threads::Threads
)
gtest_discover_tests(test_filter)
