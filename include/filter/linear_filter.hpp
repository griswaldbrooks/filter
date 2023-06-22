#ifndef FILTER_LINEAR_FILTER_H_
#define FILTER_LINEAR_FILTER_H_

#include <deque>      // For deque
#include <numeric>    // For inner_product
#include <stdexcept>  // For domain_error, length_error
#include <utility>    // For move
#include <vector>     // For vector

namespace gb::filter {

/// \tparam Coefficients number of filter elements
template <typename T>
class linear_filter {
 public:
  /// \brief Constructor for linear_filter.
  /// \param b Feedforward coefficients for the input samples.
  /// \param a Feedback coefficients for the output samples.
  /// \note The number of coefficients is the order of the filter + 1.
  /// \throws if \p b and \p a are of different lengths.
  /// \throws if the first element of \p a is not 1.
  linear_filter(std::vector<double> b, std::vector<double> a)
      : b_{std::move(b)},
        a_{std::move(a)},
        x_(b_.size(), T{}),
        y_(a_.size(), T{}) {
    if (b_.size() != a_.size()) {
      throw std::length_error{"Coefficients must have same length"};
    }
    if (a_.front() != 1.) {
      throw std::domain_error{"First element of output coefficients must be 1"};
    }
  }

  // \brief Finite impulse response constructor
  linear_filter(std::vector<double> b)
      : b_{std::move(b)},
        x_(b_.size(), T{}) {}

  /// \brief Filters value
  /// \param value Input to be filtered.
  /// \returns filtered value.
  T& operator()(const T& value) {
    // Remove the oldest elements.
    x_.pop_back();
    y_.pop_back();

    // Add the new input pose to the vector of inputs.
    x_.emplace_front(value);

    // Update the new output with the rest of the values
    const auto bx =
        std::inner_product(b_.cbegin(), b_.cend(), x_.cbegin(), T{});
    // Skip the first coefficient since it's always 1, which allows us to avoid
    // zero initializing the new output.
    const auto ay =
        std::inner_product(a_.cbegin() + 1, a_.cend(), y_.cbegin(), T{});

    // Initialize the new output and return
    return y_.emplace_front(bx - ay);
  }

 private:
  /// \brief Input coefficients
  std::vector<double> b_;
  /// \brief Output coefficients
  std::vector<double> a_;
  /// \brief Input taps
  std::deque<T> x_;
  /// \brief Previous output
  std::deque<T> y_;
};
}  // namespace gb::filter
#endif  // FILTER_LINEAR_FILTER_H_
