include(FetchContent)

set(INSTALL_GTEST OFF)
FetchContent_Declare(GTest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG v${GTest_FIND_VERSION})
FetchContent_MakeAvailable(GTest)
set_target_properties(gtest PROPERTIES COMPILE_OPTIONS "")
set_target_properties(gmock PROPERTIES COMPILE_OPTIONS "")
include(GoogleTest)
