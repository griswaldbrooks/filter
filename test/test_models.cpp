// Bring in my package's API, which is what I'm testing
#include <robot_filters/models/models.h>
// Bring in gtest
#include <gtest/gtest.h>

// Declare a test
TEST(TestSuite, testCase1)
{
}

// Declare another test
TEST(TestSuite, testCase2)
{
}

// Run all the tests that were declared with TEST()
int main(int argc, char **argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}