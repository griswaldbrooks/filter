#include "filter/linear_filter.hpp"  // For linear_filter

#include <algorithm>  // For transform
#include <cmath>      // For hypot
#include <tuple>      // For get
#include <vector>     // For vector

#include "gmock/gmock.h"                   // For EXPECT_*
#include "gtest/gtest.h"                   // For TEST_*

using ::testing::DoubleNear;
using ::testing::Pointwise;
using ::testing::TestWithParam;

namespace {
/// \brief Acceptable error between doubles
constexpr auto tolerance = 1e-4;
/// \brief Random test samples
auto const test_samples =
    std::vector<double>{0.447265, 0.677158, 0.490548, 0.896610, 0.948445,
                        0.748019, 0.050977, 0.457688, 0.448624, 0.810386};
}  // namespace

namespace gb::filter::test_filter {

struct point {
  double x, y, z;
};

point operator+(point const& lhs, point const& rhs) {
  return point{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

point operator-(point const& lhs, point const& rhs) {
  return point{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

point operator*(double s, point const& p) {
  return point{s * p.x, s * p.y, s * p.z};
}

/// \brief Compare points using euclidean distance
MATCHER_P(NearWithPrecision, precision, "") {
  auto const lhs = std::get<0>(arg);
  auto const rhs = std::get<1>(arg);
  return std::hypot(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z) < precision;
}

/// \brief Create points from doubles
/// \param values Scalars to use for each point
/// \returns points with x == y == z == value
std::vector<point> make_points(std::vector<double> const& values) {
  std::vector<point> points(values.size());
  std::transform(values.cbegin(), values.cend(), points.begin(),
                 [](auto const& value) {
                   point p;
                   p.x = p.y = p.z = value;
                   return p;
                 });
  return points;
}

/// \brief Filter configuration to test
template <typename T>
struct scenario {
  std::string display;
  std::vector<double> b;    // Input coefficients
  std::vector<double> a;    // Output coefficients
  std::vector<T> samples;   // Input to filter
  std::vector<T> expected;  // Expected filtered values
};

/// \brief Insertion operator for verbose test output
template <typename T>
std::ostream& operator<<(std::ostream& os, const scenario<T>& s) {
  return os << s.display;
}

const auto scenarios = ::testing::Values(
    // First order, 0.2 rad/sec (normalized cutoff frequency).
    scenario<double>{"first order low pass filter",
                     {0.24524, 0.24524},
                     {1.00000, -0.50953},
                     test_samples,
                     {0.10969, 0.33164, 0.45534, 0.57219, 0.74402, 0.79513,
                      0.60108, 0.43101, 0.44187, 0.53390}},
    // Second order, 0.5 rad/sec (normalized cutoff frequency).
    scenario<double>{"second order low pass filter",
                     {0.29289, 0.58579, 0.29289},
                     {1.0000e+00, -1.3878e-16, 1.7157e-01},
                     test_samples,
                     {0.13100, 0.46034, 0.64887, 0.66932, 0.83536, 0.92245,
                      0.58758, 0.22474, 0.31363, 0.59565}},
    // Third order, 0.7 rad/sec (normalized cutoff frequency).
    scenario<double>{"third order low pass filter",
                     {0.37445, 1.12336, 1.12336, 0.37445},
                     {1.00000, 1.16192, 0.69594, 0.13776},
                     test_samples,
                     {0.16748, 0.56140, 0.67795, 0.61346, 0.90503, 0.96453,
                      0.42549, 0.13377, 0.43508, 0.68341}});

/// \brief Fixture for testing linear_filter
struct LinearFilterContext : public TestWithParam<scenario<double>> {};

TEST_P(LinearFilterContext, FilterValues) {
  // GIVEN a filter
  const auto [_, b, a, samples, expected] = GetParam();
  auto filter = linear_filter<double>{b, a};

  // WHEN the inputs are filtered
  auto result = std::vector<double>(samples.size());
  std::transform(samples.cbegin(), samples.cend(), result.begin(), filter);

  // THEN they should match within tolerance.
  EXPECT_THAT(result, Pointwise(DoubleNear(tolerance), expected));
}

INSTANTIATE_TEST_SUITE_P(LinearFilterDouble, LinearFilterContext, scenarios);

TEST_F(LinearFilterContext, FilterPoints) {
  // GIVEN a first order Point filter (0.2 rad/sec (normalized cutoff frequency)
  const auto b = std::vector<double>{0.24524, 0.24524};
  const auto a = std::vector<double>{1.00000, -0.50953};
  auto filter = linear_filter<point>{b, a};

  // WHEN the inputs are filtered
  const auto samples = make_points(test_samples);
  auto result = std::vector<point>(samples.size());
  std::transform(samples.cbegin(), samples.cend(), result.begin(), filter);

  // THEN they should match within tolerance.
  const auto expected =
      make_points({0.10969, 0.33164, 0.45534, 0.57219, 0.74402, 0.79513,
                   0.60108, 0.43101, 0.44187, 0.53390});
  EXPECT_THAT(result, Pointwise(NearWithPrecision(tolerance), expected));
}

}  // namespace gb::filter::test_filter

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
